# Copyright (c) 2013 CNRS
# Author: Jorrit T'Hooft
#
# This file is part of hpp-corbaserver.
# hpp-corbaserver is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-corbaserver is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

import numpy as np
from numpy import linalg

class Quaternion (object):
    """
    Quaternion class :
    ------------------

    A quaternion has a scalar part and a vector part.
    In this class the quaternion is represented as an array of 4 elements :
      - the first element is the scalar part
      - the next 3 elements represents the vector part

    One can acces to the array directly with the attribute "array"
      e.g. q1=Quaternion(1,0,0,0) --> q1.array

    A quaternion can be instanciated with 1, 2 or 4 elements
      (see : __init__() for more information).

    It can also return a rotation vector, a rotation matrix, or a SO3
      (see the methods : to...() for more information).
    """
    def __init__(self,*args):
        """
        Instanciation of the quaternion with 1, 2 or 4 arguments  :
        -----------------------------------------------------------
        This creates a 4-sized array (self.array) representing the quaternion
        with the first element representing the scalar part
        and the 3 others the vector part.

        With 4 arguments :
        ------------------
          - the first one is used as the scalar part,
            the other three as the vector part.

        With 2 arguments :
        ------------------
          - the 1-sized argument is used as the scalar part,
            the 3-sized argument is used as the vector part.

        With 1 argument :
        -----------------
          - if it is a quaternion it will create a copy of this quaternion.
          - if it is a scalar, the scalar will be used as the scalar part
            and the vector part will be set at (0,0,0).
          - if it is an array, matrix, tuple or list of 4 elements,
            the first element is used as the scalar part
            and the rest as the vector part.
          - if it is an array, matrix, tuple or list of 3 elements,
            the 3 elements are interpreted as a rotation vector,
            this creates a quaternion representing the same rotation.
          - if it is a to 2 dimension array convertible array, matrix, tuple
            or list with at least (3*3) elements,
            the upper left (3*3) elements are interpreted as a rotation matrix,
            this creates a quaternion representing the same rotation.
          - if it is an instance of SO3, quaternion is built from rotation
            matrix.

        With 0 arguments :
        ------------------
        If no argument is given, than the quaternion will be set by default
        to with the scalar part set to 1 and the vector part to (0,0,0).
        (this is the neutral element for multiplication in quaternion space)

        To create a quaternion from Roll, Pitch, Yaw angles :
        -----------------------------------------------------
        first instanciate a quaternion and than use the method fromRPY()
        to change the values of it to the dezired ones.
          e.g. : quat().fromRPY(R,P,Y)
        """


        error=False
        if len(args)==0: # By default, if no argument is given
            self.array=np.array([1.,0.,0.,0.])
        elif len (args) == 4: # From 4 elements
            if np.array(args).size==4:
                self.array = np.double(np.array (args))
            else:
                error=True
        elif len (args) == 1:
            if type(args[0])==Quaternion: # From a Quaternion
                self.array=args[0].array.copy()
            elif np.array(args[0]).size==1: # From one sized element, this element will be the scalar part, the vector part will be set at (0,0,0)
                self.array=np.double(np.hstack([np.array(args[0]),np.array([0,0,0])]))
            elif np.array(args[0]).size==4 and max(np.array(args[0]).shape)==4: # From an array, matrix, tuple or list of 4 elements
                self.array = np.double(np.array(args[0])).reshape(4,)
            elif np.array(args[0]).size==3 and max(np.array(args[0]).shape)==3: # From an array, matrix, tuple or list of 3 elements interpreted as a rotation vector
                rV=np.double(np.array(args[0])).reshape(3,)
                alpha=np.double(linalg.norm(rV))
                if alpha !=0:
                    e=rV/alpha
                else:
                    e=rV
                self.array=np.hstack([np.cos(alpha/2.),np.sin(alpha/2.)*e])
            elif len(np.array(args[0]).shape)==2 and np.array(args[0]).shape[0]>=3 and np.array(args[0]).shape[1]>=3: # From a to 2 dimension array convertible array, matrix, tuple or list with at least (3*3) elements interpreted  as a rotation matrix
                rM=np.double(np.array(args[0])[:3,:3])
                selec=np.zeros(4)
                selec[0]=1+rM[0,0]+rM[1,1]+rM[2,2]
                selec[1]=1+rM[0,0]-rM[1,1]-rM[2,2]
                selec[2]=1-rM[0,0]+rM[1,1]-rM[2,2]
                selec[3]=1-rM[0,0]-rM[1,1]+rM[2,2]
                param=selec.argmax()
                if selec[param]>0:
                    q=np.zeros(4)
                    if param==0:
                        q[0]=np.sqrt(selec[param])
                        q[1]=(rM[2,1]-rM[1,2])/q[0]
                        q[2]=(rM[0,2]-rM[2,0])/q[0]
                        q[3]=(rM[1,0]-rM[0,1])/q[0]
                        self.array=q*0.5
                        # print '--1--V3'
                    elif param==1:
                        q[1]=np.sqrt(selec[param])
                        q[0]=(rM[2,1]-rM[1,2])/q[1]
                        q[2]=(rM[1,0]+rM[0,1])/q[1]
                        q[3]=(rM[0,2]+rM[2,0])/q[1]
                        self.array=q*0.5
                        # print '--2--V3'
                    elif param==2:
                        q[2]=np.sqrt(selec[param])
                        q[0]=(rM[0,2]-rM[2,0])/q[2]
                        q[1]=(rM[1,0]+rM[0,1])/q[2]
                        q[3]=(rM[2,1]+rM[1,2])/q[2]
                        self.array=q*0.5
                        # print '--3--V3'
                    elif param==3:
                        q[3]=np.sqrt(selec[param])
                        q[0]=(rM[1,0]-rM[0,1])/q[3]
                        q[1]=(rM[0,2]+rM[2,0])/q[3]
                        q[2]=(rM[2,1]+rM[1,2])/q[3]
                        self.array=q*0.5
                        # print '--4--V3'
                else:
                    error=True
            else:
                error=True
        elif len(args)==2: # From a scalar part (1 element) and a vector part (3 elements)
            arg0=np.double(np.array(args[0]))
            arg1=np.double(np.array(args[1]))
            if arg0.size==1 and arg1.size==3:
                self.array=np.zeros(4)
                self.array[0]=arg0
                self.array[1:4]=arg1[:]
            elif arg0.size==3 and arg1.size==1:
                self.array=np.zeros(4)
                self.array[0]=arg1
                self.array[1:4]=arg0[:]
            else:
                error=True

        else:
            error=True

        if error==False and self.array.shape!=(4,):
            del self.array
            error=True
        if error:
            raise TypeError ("Impossible to instanciate the Quaternion object with the given arguments")

    def __str__(self):
        """
        String representation of the quaternion.
        """
        aff='[ '
        aff+=str(self.array [0])+'  +  '
        aff+=str(self.array [1])+' i  +  '
        aff+=str(self.array [2])+' j  +  '
        aff+=str(self.array [3])+' k ]'
        return aff

    def __neg__(self):
        """
        Returns a quaternion which elements are the opposite of the original,
        (this opposite quaternion represents the same rotation).
        """
        return Quaternion(-self.array)

    def __add__(self,other):
        """
        If other is not a quaternion it is casted to a quaternion,
        the elements are added one to one.
        """
        if type(other)!=Quaternion:
            q2=Quaternion(other)
        else:
            q2=other
        return Quaternion(self.array+q2.array)

    def __sub__(self,other):
        """
        If other is not a quaternion it is casted to a quaternion,
        the elements are substracted one to one.
        """
        if type(other)!=Quaternion:
            q2=Quaternion(other)
        else:
            q2=other
        return Quaternion(self.array-q2.array)

    def __mul__(self,other):
        """
        If other is not a quaternion it is casted to a quaternion,
        the result of the quaternion multiplication is returned.
        """
        if type(other)!=Quaternion:
            q2=Quaternion(other)
        else:
            q2=other
        qr=np.zeros(4)
        qr[0]=self.array[0]*q2.array[0]-np.vdot(self.array[1:],q2.array[1:])
        qr[1:4]=np.cross(self.array[1:4],q2.array[1:4])+self.array[0]*q2.array[1:4]+q2.array[0]*self.array[1:4]
        return Quaternion(qr)

    def __rmul__(self,other):
        """
        other is casted to a quaternion,
        the result of the quaternion multiplication is returned.
        """
        return  Quaternion(other)*self

    def __abs__(self):
        """
        Returns the norm of the quaternion.
        """
        return np.double(linalg.norm(self.array))

    def conjugate(self):
        """
        Returns the conjugate of the quaternion.
        """
        return Quaternion(self.array[0],-self.array[1:4])

    def inv(self):
        """
        Returns the inverse of the quaternion.
        """
        return Quaternion(self.conjugate().array/(abs(self)**2))

    def __div__(self,other):
        """
        If other is not a quaternion it is casted to a quaternion,
        the result of the quaternion multiplication with the inverse of other
        is returned.
        """
        if type(other)!=Quaternion:
            q2=Quaternion(other)
        else:
            q2=other
        return self*q2.inv()

    def __pow__(self,n):
        """
        Returns quaternion**n with quaternion**0 = Quaternion(1,0,0,0).
        """
        r=Quaternion()
        for i in range(n):
                r=r*self
        return r

    def normalize (self):
        """
        Changes the values of the quaternion to make it a unit quaternion
        representing the same rotation as the original one
        and returns the updated version.
        """
        self.array /= abs(self);
        return self

    def normalized (self):
        """
        Returns the unit quaternion representation of the quaternion
        without changing the original.
        """
        qr=Quaternion(self)
        qr.normalize()
        return qr

    def toRotationMatrix(self):
        """
        Returns a (3*3) array (rotation matrix)
        representing the same rotation as the (normalized) quaternion.
        """
        q=self.normalized().array
        rm=np.zeros((3,3))
        rm[0,0]=1-2*(q[2]**2+q[3]**2)
        rm[0,1]=2*q[1]*q[2]-2*q[0]*q[3]
        rm[0,2]=2*q[1]*q[3]+2*q[0]*q[2]
        rm[1,0]=2*q[1]*q[2]+2*q[0]*q[3]
        rm[1,1]=1-2*(q[1]**2+q[3]**2)
        rm[1,2]=2*q[2]*q[3]-2*q[0]*q[1]
        rm[2,0]=2*q[1]*q[3]-2*q[0]*q[2]
        rm[2,1]=2*q[2]*q[3]+2*q[0]*q[1]
        rm[2,2]=1-2*(q[1]**2+q[2]**2)
        return rm

    def toRotationVector(self):
        """
        Returns a 3-sized array (rotation vector)
        representing the same rotation as the (normalized) quaternion.
        """
        q=self.normalized().array
        rV=np.zeros(3)
        alpha=2*np.arccos(q[0])
        if linalg.norm(q[1:4])!=0:
            rV=alpha*q[1:4]/linalg.norm(q[1:4])
        return rV

    def copy(self):
        """
        Returns a copy of the quaternion.
        """
        return Quaternion(self)

    def toRPY(self):
        """
        Returns a 3-sized array with representing the same rotation
        as the (normalized) quaternion. With :
          - the first element representing the Roll,
          - the second the Pitch
          - the third the Yaw
        Where Roll Pitch and Yaw are the angles so that the rotation
        with the quaternion represents the same rotation as :
          - A rotation of R (Roll) about the original x-axis,
            followed by a rotation of P (Pitch) about the original y-axis,
            followed by a rotation of Y (Yaw) about the original z-axis.
          - Or otherwise a rotation of Y about the original z-axis,
            followed by a rotation of P about the new y-axis,
            followed by a rotation of R about the new x-axis.
        """
        q=self.normalized().array
        r=np.arctan2(2*(q[0]*q[1]+q[2]*q[3]),1-2*(q[1]**2+q[2]**2))
        p=np.arctan2(2*(q[0]*q[2]-q[3]*q[1]),np.sqrt((2*(q[0]*q[1]+q[2]*q[3]))**2+(1-2*(q[1]**2+q[2]**2))**2)) # We cas use arcsin but arctan2 is more robust
        y=np.arctan2(2*(q[0]*q[3]+q[1]*q[2]),1-2*(q[2]**2+q[3]**2))
        return np.array([r,p,y])

    def fromRPY(self,R,P,Y):
        """
        Set the values of the quaternion to the values of a unit quaternion
        representing the same rotation as the one performed by Roll Pitch Yaw :
          - A rotation of R (Roll) about the original x-axis,
            followed by a rotation of P (Pitch) about the original y-axis,
            followed by a rotation of Y (Yaw) about the original z-axis.
          - Or otherwise a rotation of Y about the original z-axis,
            followed by a rotation of P about the new y-axis,
            followed by a rotation of R about the new x-axis.
        """
        r=R/2.
        p=P/2.
        y=Y/2.
        self.array[0]=np.cos(r)*np.cos(p)*np.cos(y)+np.sin(r)*np.sin(p)*np.sin(y)
        self.array[1]=np.sin(r)*np.cos(p)*np.cos(y)-np.cos(r)*np.sin(p)*np.sin(y)
        self.array[2]=np.cos(r)*np.sin(p)*np.cos(y)+np.sin(r)*np.cos(p)*np.sin(y)
        self.array[3]=np.cos(r)*np.cos(p)*np.sin(y)-np.sin(r)*np.sin(p)*np.cos(y)
        return self.normalize()

    def toTuple (self):
        """
        Return quaternion as a tuple a float starting with real part.
        """
        return tuple (self.array)

    def fromTwoVectors(self,a,b):
        """
        Return a quaternion build representing the rotation between the two vectors.
        In other words, the built rotation represent a rotation sending the line of direction a to the line of direction b,
         both lines passing through the origin.
         Equations from Eigen https://eigen.tuxfamily.org/dox/classEigen_1_1Quaternion.html#title13
        """
        if len(a) != 3 or len(b) != 3:
            raise TypeError ("fromTwoVector method require two 3D vectors as input.")

        v0 = np.array(a)
        n0 = np.linalg.norm(v0)
        v1 = np.array(b)
        n1 = np.linalg.norm(v1)
        if n0 < (np.finfo(np.float32).eps) or n1 < (np.finfo(np.float32).eps) :
          return self
        v0 = v0 / n0
        v1 = v1 / n1
        c = v1.dot(v0)

        if c > -1. + (np.finfo(np.float32).eps) :
          axis = np.cross(v0,v1)
          s = np.sqrt((1. + c)*2.)
          invs = 1./s
          self.array[0:3] = axis*invs
          self.array[3] = 0.5*s
          return self.normalize()
        else :
        # both vectors are nearly opposite, previous method may lead to numerical error.
        # here we accurately compute the rotation axis by computing the intersection between two planes :
        # x^T v1 = 0
        # x^T v0 = 0
        # ||x|| = 1
        # which lead to a singular value problem
          c = max(c,-1.)
          m = np.matrix([v0,v1]).transpose()
          u,s,vh  = np.linalg.svd(m)
          axis = u[:,2].transpose()
          w2 = (1.+c)*0.5
          self.array[0:3] = axis*np.sqrt(1.-w2)
          self.array[3] = np.sqrt(w2)
          return self.normalize()
