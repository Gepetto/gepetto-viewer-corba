#!/usr/bin/env python
#
# Copyright (c) 2015 CNRS
# Author: Steve Tonneau, Joseph Mirabel
#
# This file is part of hpp-gepetto-viewer.
# hpp-gepetto-viewer is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-gepetto-viewer is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-gepetto-viewer.  If not, see
# <http://www.gnu.org/licenses/>.

###
# See blender/README.md for instructions
# https://github.com/jmirabel/gepetto-viewer-corba/tree/devel/blender
###

bl_info = {
    "author": "Joseph Mirabel",
    "name"  : "Gepetto Viewer Blender Addon",
    "category": "Import-Export",
    "blender": (2,75,0),
    "description": "Add functionality to import files generated using the Gepetto Viewer software",
    "location": "SpaceBar Search > YAML Gepetto or URDF blender",
    "wiki_url": "https://github.com/jmirabel/gepetto-viewer-corba/tree/master/blender#readme",
    "warning" : "Not heavily tested, feel free to report bug on github.",
    "version" : (0,0),
    "support" : "COMMUNITY"
    }

import bpy
import bpy_extras.io_utils
import re
import yaml, os

def loadmotion (filename):
    with open (filename) as file:
        data = yaml.load (file)
        for frameId in range (len(data.keys())):
            frameKey = "frame_" + str (frameId)
            objPositions = data[frameKey]
            for objName, pos in objPositions.items ():
                currentObj = bpy.context.scene.objects.get(objName)
                if currentObj:
                    currentObj.rotation_mode = 'QUATERNION'
                    posF = [float(x) for x in pos]
                    if len(posF)>7:
                        currentObj.dimensions[0]=posF[7]
                        currentObj.keyframe_insert(data_path="scale",frame=frameId)
                        if posF[7] <= 0:
                            posF[3:7]=[1,0,0,0]
                    currentObj.location = posF[0:3]
                    currentObj.rotation_quaternion = posF[3:7]
                    currentObj.keyframe_insert (data_path="location", frame=frameId)
                    currentObj.keyframe_insert (data_path="rotation_quaternion", frame=frameId)
                else:
                    print("Unknown object " + objName)

def checkframe (filename, frameId):
    with open (filename) as file:
        data = yaml.load (file)
        frameKey = "frame_" + str (frameId)
        objPositions = data[frameKey]
        for objName, pos in objPositions.items ():
            currentObj = bpy.context.scene.objects.get(objName)
            if currentObj:
                currentObj.rotation_mode = 'QUATERNION'
                posF = [float(x) for x in pos]
                currentObj.location = posF[0:3]
                currentObj.rotation_quaternion = posF[3:7]
            else:
                print("Unknown object " + objName)

class YamlPathImport (bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    bl_idname = "import.gepettoimport"
    bl_label = "Import a YAML Gepetto Viewer path file"

    files = bpy.props.CollectionProperty(name="File Path", type=bpy.types.OperatorFileListElement,)
    directory = bpy.props.StringProperty(subtype='DIR_PATH',)
    
    def execute(self, context):
        dir = self.directory
        for f in self.files:
            fullname = os.path.join (dir, f.name)
            self.report ({'INFO'}, "Loading " + str(fullname))
            loadmotion(fullname)
        return {'FINISHED'}
    
    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

class UrdfToBlendImport (bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    bl_idname = "import.urdf_to_blendimport"
    bl_label = "Import a URDF blender script"
    
    files = bpy.props.CollectionProperty(name="File Path", type=bpy.types.OperatorFileListElement,)
    directory = bpy.props.StringProperty(subtype='DIR_PATH',)
    
    def execute(self, context):
        dir = self.directory
        for f in self.files:
            fullname = os.path.join (dir, f.name)
            self.report ({'INFO'}, "Loading " + str(fullname))
            exec(open(fullname).read())
        return {'FINISHED'}
    
    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

def register():
    bpy.utils.register_class(YamlPathImport)
    bpy.utils.register_class(UrdfToBlendImport)

def unregister():
    bpy.utils.unregister_class(YamlPathImport)
    bpy.utils.unregister_class(UrdfToBlendImport)

if __name__ == "__main__":
    register()
