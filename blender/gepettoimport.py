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
