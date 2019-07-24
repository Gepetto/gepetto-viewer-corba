from . import Color

import numpy as np
from math import pi, sqrt, cos, sin

def _pointsTorus(R, r, nR, nr):
    pts = []
    twopi = 2 * pi
    for j in range(nR):
        for i in range(nr + 1):
            s = i % nr
            phi = s* twopi /nr
            for k in (1, 0):
                t = (j + k) % nR

                theta = t* twopi /nR
                y =  (R+r*cos(phi))*cos(theta);
                z = -(R+r*cos(phi))*sin(theta);
                x = r * sin(phi);
                pts.append ((x, y, z))
    return pts

def _pointsCone(r, L, nr, R):
    pts = []
    twopi = 2 * pi
    for j in range(nr+1):
        t = j % nr
        theta = t* twopi /nr
        pts.append ((0., R, L))
        pts.append ((r*cos(theta), R + r*sin(theta), 0.))
    return pts

def _pointsCircularArrow (R, r, nR, nr):
    ptsCone =  _pointsCone (1.5*r, 3*r, nr, R)
    ptsTorus = _pointsTorus (R, r, nR, nr)
    return ptsCone + ptsTorus, len(ptsCone)

# Compute a transformation which rotates (1, 0, 0) to v
# Return (norm_v, T)
# When norm_v == 0, T is None
def _tranformFromXvector (v):
    norm_v = np.linalg.norm(v)
    if norm_v == 0:
        # self.plugin.gui.gui.resizeArrow (n, self.radius, norm_v)
        return 0, None
    # x = (1, 0, 0)
    u = v / norm_v
    if u[0] < -1 + 1e-6: # Nearly opposite vectors
        m = np.array ( ( (1, 0, 0), u ) )
        U, S, V = np.linalg.svd (m)
        c = max (u[0], -1)
        w2 = (1 + c) / 2
        s = sqrt(1 - w2)
        t = (0,0,0, V[0,2]/s, V[1,2]/s, V[2,2]/s, w2)
    else:
        s = sqrt(2 * (1 + u[0]))
        # axis = (0, -u[2]/s, u[1]/s) # x ^ u
        t = (0,0,0, 0, -u[2]/s, u[1]/s, s/2)
    return norm_v, t

class Angular:
    ## \param majorRadius, majorN radius and discretisation of the circle that is
    ##                            the center of the torus
    ## \param minorRadius, minorN radius and discretisation of the circle that is
    ##                            a section of the torus
    def __init__ (self, nodeName, color = Color.red, majorRadius = 0.1, minorRadius = 0.005, majorN = 100, minorN = 20):
        self.name = nodeName
        self.R = majorRadius
        self.r = minorRadius
        self.nR = majorN
        self.nr = minorN
        self.color = color

    def create (self, gui):
        pts, self.minC = _pointsCircularArrow (self.R, self.r, self.nR, self.nr)
        gui.addCurve (self.name, pts, self.color)
        gui.setCurveMode (self.name, "quad_strip")
        self.maxC = len(pts)
    def remove (self, gui):
        gui.deleteNode (self.name, False)

    ## \param factor the norm of the vector is divided by this value
    def set(self, gui, v, factor = pi):
        norm, pos = _tranformFromXvector (v)
        count = min (self.minC + int (norm * (self.maxC-self.minC) / factor), self.maxC)
        gui.setCurvePointsSubset (self.name, 0, count)
        if pos is not None: gui.applyConfiguration (self.name, pos)

class Linear:
    ## \param majorRadius, majorN radius and discretisation of the circle that is
    ##                            the center of the torus
    ## \param minorRadius, minorN radius and discretisation of the circle that is
    ##                            a section of the torus
    def __init__ (self, nodeName, color = Color.red, radius = 0.005):
        self.name = nodeName
        self.R = radius
        self.color = color

    def create (self, gui):
        gui.addArrow (self.name, self.R, 1, self.color)
    def remove (self, gui):
        gui.deleteNode (self.name, False)

    ## \param factor the norm of the vector is divided by this value
    def set(self, gui, v, factor = 1.):
        norm, pos = _tranformFromXvector (v)
        gui.resizeArrow (self.name, self.R, norm / factor)
        if pos is not None: gui.applyConfiguration (self.name, pos)


## Represents a 6D vector using an arrow with the 3 first
## values and a circular arrow with the 3 last values.
## It is useful to plot velocities of a body, forces and torques...
class Vector6:
    suffixLin = "linear"
    suffixAng = "angular"

    ## \param groupName name of an **existing** group
    def __init__ (self, groupName, angFactor = pi, linFactor = 1.):
        self.name = groupName
        self.linear  = Linear  (self.name + '/' + Vector6.suffixLin)
        self.angular = Angular (self.name + '/' + Vector6.suffixAng)
        self.angF = angFactor
        self.linF = linFactor

    def create (self, gui):
        self.linear .create(gui)
        self.angular.create(gui)

    def remove (self, gui):
        self.linear .remove(gui)
        self.angular.remove(gui)

    def set (self, gui, v):
        self.linear .set (gui, v[0:3], self.linF)
        self.angular.set (gui, v[3:6], self.angF)

## Plot a function of 2 arguments i.e. (x, y, f(x, y))
# \param gui the CORBA client
# \param name the curve node name
# \param args a list [X,Y] of two lists X and Y, where the arguments that should
#             be considered are all the possible pairs of elements of X and Y.
# \param func a function that takes x,y as arguments
# \param color
def graphOfFunction(gui, name, args, func, color=Color.red):
    # Generate the
    def _recursiveBuildPoints(arg, iarg, args, func):
        if iarg == len(args):
            return (func(arg),)
        points = []
        for k in range(len(args[iarg])):
            cur = arg[:]
            cur.append(args[iarg][k])
            points.extend (_recursiveBuildPoints(cur, iarg+1, args, func))
        return points

    # Build the vector of points
    points = _recursiveBuildPoints([], 0, args, func)
    if len(args) == 2:
        # Duplicate the points to have triangles
        nx = len(args[0])
        ny = len(args[1])
        pts = []
        for ix in range(nx-1):
            for iy in range(ny-1):
                iv0 = ix * ny + iy
                iv1 = ix * ny + iy + 1
                iv2 = (ix + 1) * ny + iy
                iv3 = (ix + 1) * ny + iy + 1
                pts.append(points[iv0])
                pts.append(points[iv1])
                pts.append(points[iv2])
                pts.append(points[iv1])
                pts.append(points[iv2])
                pts.append(points[iv3])

        gui.addCurve(name, pts, color)
        gui.setCurveMode(name, "triangles")
    else:
        gui.addCurve(name, points, color)
        gui.setCurveMode(name, "line_strip")

    return points
