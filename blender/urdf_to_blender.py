#!/usr/bin/env python2.7
from __future__ import print_function
import roslib; roslib.load_manifest('urdf_parser_py')
import rospy
import resource_retriever
import sys, getopt
import os.path
import inspect

import urdf_parser_py.urdf as urdf

def usage ():
    print (os.path.basename(sys.argv[0]) + " [--env] [-p <prefix>] -i <urdf-file> -o <blender-script>\n")
    print ("Arguments:")
    print ("\t-i urdf-file     \t[mandatory]\tinput URDF file")
    print ("\t-o blender-script\t[mandatory]\toutput blender script")
    print ("\t-p prefix        \t[optional] \tprefix of object names")
    print ("\t--env            \t[optional] \tobject static transform will be ignored")

try:
    opts, args = getopt.getopt (sys.argv[1:], "p:i:o:", ["env", "prefix=", "in=", "out="])
except getopt.GetoptError as err:
    usage ()
    print ("\nError: " + err.msg + "\n")
    raise

prefix = ""
urdfFilename = None
blendFilename = None
isEnv = False
for opt, arg in opts:
    if opt in ("-i", "--in"):
        urdfFilename = arg
    elif opt in ("-o", "--out"):
        blendFilename = arg
    elif opt in ("-p", "--prefix"):
        prefix = arg
    elif opt in ("--env"):
        isEnv = True

if urdfFilename is None or blendFilename is None:
    usage ()
    sys.exit (2)

## There is a bug in urdf_parser_py that has been fixed in a later release.
## See https://github.com/ros/urdfdom/pull/38
## and https://github.com/ros/urdfdom/pull/46/files
for tag in urdf.JointLimit.XML_REFL.attributes:
    if tag.var == 'upper' or tag.var == 'lower':
        tag.required = False
        tag.default = 0

## There is something wrong with tag transmission
## This just disables it but it is not the right way of doing it.
urdf.Transmission.XML_REFL.attributes = list()
urdf.Transmission.XML_REFL.vars = list()
urdf.Transmission.XML_REFL.required_attribute_names = list()
urdf.Transmission.XML_REFL.required_element_names = list()
urdf.Transmission.XML_REFL.attribute_map = dict()
urdf.Transmission.XML_REFL.element_map = dict()
urdf.Transmission.XML_REFL.scalarNames = list()
urdf.Transmission.XML_REFL.scalars = list()

robot = urdf.URDF.from_xml_file (urdfFilename)

def resolve_ros_path (path):
    filename = resource_retriever.get_filename (path)
    if filename.startswith ("file://"):
        return filename[7:]
    else:
        print ("Path might not be understood by blender: " + filename)
        return filename

def updateFrameMessage ():
    callerframerecord = inspect.stack()[1]
    info = inspect.getframeinfo(callerframerecord[0])
    print ("Update function %s in script %s:%i" % (info.function, info.filename, info.lineno))

class CreateBlenderObject:
    def __init__ (self, prefix, file):
        self.run = dict ()
        self.file = file
        self.prefix = prefix
        self.materials = list ()
        self.textures = list ()
        self.run[urdf.Cylinder] = self.handleCylinder
        self.run[urdf.Box] = self.handleBox
        self.run[urdf.Sphere] = self.handleSphere
        self.run[urdf.Mesh] = self.handleMesh
        self.writeCmd ("import bpy")
        self.writeCmd ("""
taggedObjects = list()
def tagObjects ():
  global taggedObjects
  taggedObjects = list ()
  for obj in bpy.data.objects:
    taggedObjects.append (obj.name)

def getNonTaggedObjects ():
  global taggedObjects
  return [obj for obj in bpy.data.objects if obj.name not in taggedObjects]

def setParent (children, parent):
  for child in children:
    child.parent = parent
""")

    def setName (self, name):
        if isEnv:
            self.writeCmd ("bpy.context.object.name = \"" + self.prefix + name + "_0\"")
        else:
            self.writeCmd ("bpy.context.object.name = \"" + self.prefix + name + "_visual0\"")

    def setupParent (self, name):
        self.writeCmd ("bpy.ops.object.empty_add ()")
        self.writeCmd ("empty = bpy.context.object")
        self.writeCmd ("empty.name = \"" + self.prefix + name + "\"")
        self.writeCmd ("currentObj.parent = empty")

    def translate (self, position):
        self.writeCmd ("currentObj.location = %s" % \
                (position, ))

    def rotate (self, rotation):
        self.writeCmd ("currentObj.rotation_euler = %s" % \
                (rotation, ))

    def scale (self, scale):
        self.writeCmd ("currentObj.scale = %s" % \
                (scale, ))

    def handleSphere (self, sphere):
        print ("Untested feature: Sphere will be treated as icosphere")
        self.writeCmd ("bpy.ops.mesh.primitive_ico_sphere_add (size=%s)" %\
                (sphere.radius,))
        self.writeCmd ("currentObj = bpy.context.object")

    def handleBox (self, geometry):
        self.writeCmd ("bpy.ops.mesh.primitive_cube_add ()")
        self.writeCmd ("currentObj = bpy.context.object")
        self.writeCmd ("currentObj.dimensions = %s" %\
                (geometry.size, ))

    def handleCylinder (self, geometry):
        self.writeCmd ("bpy.ops.mesh.primitive_cylinder_add (radius=%s, depth=%s)" % \
                (geometry.radius, geometry.length, ))
        self.writeCmd ("currentObj = bpy.context.object")

    def handleMesh (self, geometry):
        self.writeCmd ("tagObjects()")
        extension = os.path.splitext(geometry.filename)[1]
        if extension.lower() == '.dae':
            command = "bpy.ops.wm.collada_import (filepath=\"%s\")"
        elif extension.lower() == '.stl':
            command = "bpy.ops.import_mesh.stl (filepath=\"%s\")"
        else:
            command = "bpy.ops.mesh.primitive_cube_add () # Failed to find loading method for %s"
            print ("Extension %s of file %s is not know by the script" %\
                    (extension, geometry.filename, ))
            updateFrameMessage ()
        self.writeCmd (command % (resolve_ros_path (geometry.filename),))
        self.writeCmd ("imported_objects = getNonTaggedObjects ()")
        self.writeCmd ("print(imported_objects)")
        self.writeCmd ("bpy.ops.object.empty_add ()")
        self.writeCmd ("currentObj = bpy.context.object")
        self.writeCmd ("setParent (imported_objects, currentObj)")

    def writeCmd (self, command):
        print(command, file=self.file)

    def addMaterial (self, name, rgba):
        self.writeCmd ("mat = bpy.data.materials.new(\"%s\")" % (name,))
        self.writeCmd ("mat.diffuse_color = %s" % (rgba[0:3],))
        self.writeCmd ("mat.alpha = %s" % (rgba[3],))
        self.materials.append(name)

    def addTexture (self, name, filename):
        self.writeCmd ("img = bpy.data.images.load (\"%s\")" % (filename,))
        self.writeCmd ("cTex = bpy.data.textures.new(\"%s\", type='IMAGE')" % (name,))
        self.writeCmd ("cTex.image = img")
        self.writeCmd ("mat = bpy.data.materials.new(\"%s\")" % (name,))
        self.writeCmd ("mtex = mat.texture_slots.add()")
        self.writeCmd ("mtex.texture = cTex")
        self.writeCmd ("mtex.texture_coords = 'ORCO'")
        self.textures.append(name)
        self.materials.append(name)

    def setMatOrText (self, name):
        if name in self.textures:
            # self.writeCmd ("bpy.context.object.data.textures.append(bpy.data.textures[\"%s\"])" %\
                    # (name,))
                    pass
        if name in self.materials:
            self.writeCmd ("bpy.context.object.data.materials.append(bpy.data.materials[\"%s\"])" %\
                    (name,))

    def __call__ (self, link):
        geometry = link.visual.geometry
        if self.run.has_key (type(geometry)):
            self.run[type(geometry)](geometry)
            self.setName(link.name)
            if link.visual.material is not None:
                self.setMatOrText (link.visual.material.name)
            self.setupParent (link.name)
            if link.visual.origin is not None:
                self.translate (link.visual.origin.position)
                self.rotate    (link.visual.origin.rotation)
            # if link.visual.geometry.scale is not None:
                # self.scale (link.visual.geometry.scale)
                # pass
        else:
            print ("Geometry " + str(type(geometry)) + " not supported")
            updateFrameMessage ()

with open (blendFilename, "w+") as blendscript:
    blend = CreateBlenderObject (prefix, blendscript)
    for m in robot.materials:
        if m.color is not None:
            blend.addMaterial (m.name, m.color.rgba)
        if m.texture is not None:
            blend.addTexture (m.name, resolve_ros_path (m.texture.filename))
    for link in robot.links:
        if link.visual is not None:
            blend (link)
