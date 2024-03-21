from math import cos, pi, sin, sqrt

import numpy as np

from . import Color


def _pointsTorus(R, r, nR, nr):
    pts = []
    twopi = 2 * pi
    for j in range(nR):
        for i in range(nr + 1):
            s = i % nr
            phi = s * twopi / nr
            for k in (1, 0):
                t = (j + k) % nR

                theta = t * twopi / nR
                y = (R + r * cos(phi)) * cos(theta)
                z = -(R + r * cos(phi)) * sin(theta)
                x = r * sin(phi)
                pts.append((x, y, z))
    return pts


def _pointsCone(r, L, nr, R):
    pts = []
    twopi = 2 * pi
    for j in range(nr + 1):
        t = j % nr
        theta = t * twopi / nr
        pts.append((0.0, R, L))
        pts.append((r * cos(theta), R + r * sin(theta), 0.0))
    return pts


def _pointsCircularArrow(R, r, nR, nr):
    ptsCone = _pointsCone(1.5 * r, 3 * r, nr, R)
    ptsTorus = _pointsTorus(R, r, nR, nr)
    return ptsCone + ptsTorus, len(ptsCone)


def _tranformFromXvector(v, origin=None):
    """
    Compute a transformation which rotates (1, 0, 0) to v
    Return (norm_v, T)
    When norm_v == 0, T is None
    """
    norm_v = np.linalg.norm(v)
    if norm_v == 0:
        # self.plugin.gui.gui.resizeArrow (n, self.radius, norm_v)
        return 0, None
    # x = (1, 0, 0)
    u = v / norm_v
    if u[0] < -1 + 1e-6:  # Nearly opposite vectors
        m = np.array(((1, 0, 0), u))
        U, S, V = np.linalg.svd(m)
        c = max(u[0], -1)
        w2 = (1 + c) / 2
        s = sqrt(1 - w2)
        t = origin + (V[0, 2] / s, V[1, 2] / s, V[2, 2] / s, w2)  # noqa: RUF005
    else:
        s = sqrt(2 * (1 + u[0]))
        # axis = (0, -u[2]/s, u[1]/s) # x ^ u
        t = origin + (0, -u[2] / s, u[1] / s, s / 2)  # noqa: RUF005
    return norm_v, t


class Angular:
    """Helper class to plot a 3D vector as a circular arrow."""

    def __init__(
        self,
        nodeName,
        color=Color.red,
        majorRadius=0.1,
        minorRadius=0.005,
        majorN=100,
        minorN=20,
    ):
        """
        Constructor
        \\param majorRadius, majorN radius and discretisation of the circle that is
                                   the center of the torus
        \\param minorRadius, minorN radius and discretisation of the circle that is
                                   a section of the torus
        """
        self.name = nodeName
        self.setTorusParameters(majorRadius, minorRadius, majorN, minorN)
        self.color = color

    def setTorusParameters(self, majorRadius, minorRadius, majorN, minorN):
        """
        Set the torus parameters.
        \\param majorRadius, majorN radius and discretisation of the circle that is
                                   the center of the torus
        \\param minorRadius, minorN radius and discretisation of the circle that is
                                   a section of the torus
        """
        self.R = majorRadius
        self.r = minorRadius
        self.nR = majorN
        self.nr = minorN

    def create(self, gui):
        """Create the node in the GUI."""
        pts, self.minC = _pointsCircularArrow(self.R, self.r, self.nR, self.nr)
        gui.addCurve(self.name, pts, self.color)
        gui.setCurveMode(self.name, "quad_strip")
        self.maxC = len(pts)

    def remove(self, gui):
        """Delete the node in the GUI."""
        gui.deleteNode(self.name, False)

    def set(self, gui, value, origin=(0, 0, 0), factor=pi):
        """
        Set the 3D vector to plot
        \\param gui    the IDL client (typically the \\c gui attribute
                                       of class gepetto.corbaserver.client.Client)
        \\param value  the vector to plot (list or tuple of 3 elements)
        \\param origin the translation of the origin (list or tuple of 3 elements)
        \\param factor the norm of the vector is divided by this value
        """
        norm, pos = _tranformFromXvector(value, origin=origin)
        count = min(self.minC + int(norm * (self.maxC - self.minC) / factor), self.maxC)
        gui.setCurvePointsSubset(self.name, 0, count)
        if pos is not None:
            gui.applyConfiguration(self.name, pos)


class Linear:
    """Helper class to plot a 3D vector as a straight arrow."""

    def __init__(self, nodeName, color=Color.red, radius=0.005):
        """
        Constructor
        \\param radius the radius of the arrow.
        """
        self.name = nodeName
        self.R = radius
        self.color = color

    def setRadius(self, radius):
        """
        Set the arrow parameter.
        \\param radius the radius of the arrow.
        """
        self.R = radius

    def create(self, gui):
        """Create the node in the GUI."""
        gui.addArrow(self.name, self.R, 1, self.color)

    def remove(self, gui):
        """Delete the node in the GUI."""
        gui.deleteNode(self.name, False)

    def set(self, gui, value, origin=(0, 0, 0), factor=1.0):
        """
        Set the 3D vector to plot
        \\param gui    the IDL client (typically the \\c gui attribute
                                       of class gepetto.corbaserver.client.Client)
        \\param value  the vector to plot (list or tuple of 3 elements)
        \\param origin the translation of the origin (list or tuple of 3 elements)
        \\param factor the norm of the vector is divided by this value
        """
        norm, pos = _tranformFromXvector(value, origin=origin)
        gui.resizeArrow(self.name, self.R, norm / factor)
        if pos is not None:
            gui.applyConfiguration(self.name, pos)


class Vector6:
    """
    Helper class to plot a 6D vector with one Linear and one Angular arrow.
    The 3 first components of the 6D vector are sent to the Linear arrow
    and the 3 last ones are sent to the Angular arrow.
    It is useful to plot velocities of a body, forces and torques...
    Example usage:
    \\code
    from gepetto.corbaserver import Client
    from gepetto.corbaserver.tools import Vector6

    client = Client()
    gui = client.gui
    gui.createWindow ("window")
    vector = Vector6 ("window")
    vector.create (gui)
    vector.set (gui, [.1, .2, .3, .1, -.2, .3])
    \\endcode
    """

    suffixLin = "linear"
    suffixAng = "angular"

    def __init__(self, groupName, angFactor=pi, linFactor=1.0):
        """
        Constructor
        To tune the parameter of the 3D arrows, directly acces the attributes:
        - Vector6.linear
        - Vector6.angular
        - Vector6.linF
        - Vector6.angF
        \\param groupName name of an **existing** group
        \\param angFactor the norm of the angular part of the vector is divided by
               this value
        \\param linFactor the norm of the linear part of the vector is divided by
               this value
        """
        self.name = groupName
        # The Linear part
        self.linear = Linear(self.name + "/" + Vector6.suffixLin)
        # The Angular part
        self.angular = Angular(self.name + "/" + Vector6.suffixAng)
        # The factor sent to Angular.set
        self.angF = angFactor
        # The factor sent to Linear.set
        self.linF = linFactor

    def create(self, gui):
        """Create the node in the GUI."""
        self.linear.create(gui)
        self.angular.create(gui)

    def remove(self, gui):
        """Delete the node in the GUI."""
        self.linear.remove(gui)
        self.angular.remove(gui)

    def set(self, gui, value, origin=(0, 0, 0)):
        """
        Set the 6D vector to plot
        \\param gui    the IDL client (typically the \\c gui attribute
                                       of class gepetto.corbaserver.client.Client)
        \\param value  the vector to plot (list or tuple of 6 elements)
        \\param origin the translation of the origin (list or tuple of 3 elements)
        """
        self.linear.set(gui, value[0:3], factor=self.linF, origin=origin)
        self.angular.set(gui, value[3:6], factor=self.angF, origin=origin)


def graphOfFunction(gui, name, args, func, color=Color.red):
    """
     Plot a function of 2 arguments i.e. (x, y, f(x, y))
    \\param gui the CORBA client
    \\param name the curve node name
    \\param args a list [X,Y] of two lists X and Y, where the arguments that should
                be considered are all the possible pairs of elements of X and Y.
    \\param func a function that takes x,y as arguments
    \\param color
    """

    def _recursiveBuildPoints(arg, iarg, args, func):
        if iarg == len(args):
            return (func(arg),)
        points = []
        for k in range(len(args[iarg])):
            cur = arg[:]
            cur.append(args[iarg][k])
            points.extend(_recursiveBuildPoints(cur, iarg + 1, args, func))
        return points

    # Build the vector of points
    points = _recursiveBuildPoints([], 0, args, func)
    if len(args) == 2:
        # Duplicate the points to have triangles
        nx = len(args[0])
        ny = len(args[1])
        pts = []
        for ix in range(nx - 1):
            for iy in range(ny - 1):
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
