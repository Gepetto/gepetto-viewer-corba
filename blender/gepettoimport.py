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

import bpy
import bpy_extras.io_utils
import re
import yaml

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
    
    filepath = bpy.props.StringProperty(subtype="FILE_PATH")
    
    def execute(self, context):
        self.report ({'INFO'}, "Loading " + str(self.filepath))
        loadmotion(self.filepath)
        return {'FINISHED'}
    
    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

class UrdfToBlendImport (bpy.types.Operator, bpy_extras.io_utils.ImportHelper):
    bl_idname = "import.urdf_to_blendimport"
    bl_label = "Import a script generated with urdf_to_blend.py"
    
    filepath = bpy.props.StringProperty(subtype="FILE_PATH")
    
    def execute(self, context):
        self.report ({'INFO'}, "Loading " + str(self.filepath))
        exec(open(self.filepath).read())
        return {'FINISHED'}
    
    def invoke(self, context, event):
        context.window_manager.fileselect_add(self)
        return {'RUNNING_MODAL'}

bpy.utils.register_class(YamlPathImport)
bpy.utils.register_class(UrdfToBlendImport)
