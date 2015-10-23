#!/usr/bin/env python
#
# Copyright (c) 2014 CNRS
# Author: Steve Tonneau
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


######################################################
#
# Script for importing a motion computed with gepetto viewer to blender
#
###################################################


#  A. Exporting the empty scene ("without the robot")
#  This is already possible from the current python API, by simply calling
#  the method "writeNodeFile" defined in hpp.gepetto.viewer
#  
#  Once the scene is loaded, simply type
#  
#  gui.writeNodeFile(0, 'scene.obj')
#  I recommend the obj format. The collada sometimes fails for some reason.
#  
#  
#  B.1 For non hpp users: recording frames
#  
#  - first, make sure that all the objects related to your robot belong to
#  the same group, say "hrp2".
#  
#  For any frame for which you want to record the configuration of the
#  robot, you need to call the method
    #~ exportState(viewer, group, outData)
#  
#  defined in gepetto.corbaserver.exporttoblender
#  
#  where viewer is the instance of the gepetto viewer, group is the string
#  designing the group of your robot, and outData is a dictionnary that
#  will be filled with relevant data.
#  
#  
#  for instance to record 100 frames:
#  
#  import gepetto.corbaserver.exporttoblender as etb
#  
#  group = "hrp2"
#  data = {}
#  
#  for i in range(0,100):
#  	   #update robot configuration
#       ...
# 	   etb.exportState(viewer, group, data)
#  
#  #then you can export the motion
#   etb.writeDataToFile(group, data, "filename.txt")
#  
#  
#  B.2 For hpp users: recording frames / a list of configurations
#  
#  In the module hpp.gepetto.blender.exportmotion
#  you can directly export a motion by calling either
#  
#  exportPath(viewer, robot, problem, pathId, stepsize, filename)
#  where robot is the name of your robot, problem is the python problem
#  instance, pathId is the id of the computed path you want to export,
#  and stepsize is the discretization step used to generate discrete
#  postures along the path
#  
#  or
#  
#  exportStates(viewer, robot, configurations, filename)
#  where robot is the name of your robot, and configurations is a list of
#  configurations
#  
#  
#  C. Loading the motion into blender
#  
#  This step assumes that you have loaded, in blender, a scene containing
#  all the objects, the names of which correspond to the links of the
#  robot, as defined in their urdf file.
#  I already computed such files for Hyq and HRP-2, and can give them to
#  you if you signed the non disclosure agreement. Olivier,
#  it might be nice to add them to the hrp2_description later?
#  
#  C.1 load the motion.
#  Switch to the script view in blender (menu on the top the screen)
#  open the python script that you can find in the gepetto-viewer
#  repository, under blender/gepettoimport.py
#  
#  Experimented users can install the script as an add-on to blender.
#  
#  Run the script, by calling the loadmotion method with the path to the
#  previously generated file in parameter.
#  
#  Voilà! A set of frames should automatically be generated, resulting
#  in an animation.
#  
#  If some errors occur, the messages will appear in the console, or the
#  terminal used to launch blender, depending on how the script was launched.
#  
#  C.2 You can now import the previsouly generated scene using the import
#  function from the file menu. 
  
import bpy
import re


def loadmotion (filename):
	file = open(filename,"r+");
	# first retrieve frame range
	frameLine = file.readline();
	assert (frameLine.find('nbFrames=') != -1),"Invalid file format. First line must be of type: 'nbFrames="
	_, nbFrames=frameLine.rstrip("\n").split("="); nbFrames = (int)(nbFrames)
	unknownObjects = []
	for line in file.readlines():
		if not (line.find('OBJECT=') == -1):
			_, obj = line.rstrip("\n").split("=");
			currentObj = bpy.context.scene.objects.get(obj)
			if currentObj:
				objset = True
				currentObj.rotation_mode = 'QUATERNION'
			else:
				unknownObjects.append(obj)
				objset = False
		elif re.match('\d+=', line) != None and objset:
			frameId, data = line.rstrip("\n").split("=");
			x, y, z, q0, q1, q2, q3 = data.split(", ")
			bpy.context.scene.frame_set(int(frameId)) # frame from file name or loop variable
			bpy.context.scene.objects.active = currentObj
			currentObj.location = (float(x), float(y), float(z))
			currentObj.rotation_quaternion = (float(q0), float(q1), float(q2), float(q3))
			bpy.ops.anim.keyframe_insert(type='BUILTIN_KSI_LocRot')
	if len(unknownObjects) >0:
		print('unknown objects:\n')
		for uk in unknownObjects:
			print ('\t'+uk+'\n')
