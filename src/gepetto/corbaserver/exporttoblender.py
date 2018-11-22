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
# Script for exporting a motion computed with gepetto viewer to blender
#
###################################################

## Export object position for a given robot configuration
#
# \param viewer gepetto viewer instance
# \param group object group considered,
# \param configuration current robot configuration
# \param outData array containing each object position, indexed first by object then by frame
def exportState(viewer, group, outData):
	gui = viewer.client.gui
	objNames = set([])
	#retrieve object names
	for obj in gui.getGroupNodeList(group):
		objNames.add(obj)
	while len(objNames) > 0:
		obj = objNames.pop()
		if obj not in outData:
			outData[obj] = []
		objFrame = outData[obj]
		objFrame.append(gui.getNodeGlobalTransform(obj))

## Export object position for a given robot configuration
#
# \param outData data computed by the exportState calls
# \param filename name of the output file where to save the output
def writeDataToFile(group, outData, filename):
	outFile = open(filename, "w+")
	#write number of frames
	outFile.write('nbFrames='+str(len(outData[outData.keys()[0]]))+'\n')
	for obj, frames in outData.items():
		outFile.write('OBJECT='+obj[len(group)+1:]+'\n')
		for frame in range(0,len(frames)):
			outFile.write(str(frame)+'='+str(frames[frame]).lstrip("[").rstrip("]")+'\n')
	outFile.close()
