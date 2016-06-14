#!/usr/bin/env python

# Copyright (c) 2014 CNRS
# Author: Joseph Mirabel
#
# This file is part of gepetto-viewer-corba.
# gepetto-viewer-corba is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# gepetto-viewer-corba is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# gepetto-viewer-corba.  If not, see
# <http://www.gnu.org/licenses/>.

## Helper class 
class Color(object):
    # Define some RGBA-normalized color (osg convention)
    white       = [1.0,1.0,1.0,1.0]
    lightWhite  = [1.0,1.0,1.0,0.5]
    green       = [0,1,0,1]
    lightGreen  = [0,1,0,0.5]
    yellow      = [1,1,0,1]
    lightYellow = [1,1,0,0.5]
    blue        = [0.0, 0.0, 1, 1.0]
    lightBlue   = [0.0, 0.0, 1, 0.5]
    grey        = [0.7,0.7,0.7,1.0]
    lightGrey   = [0.7,0.7,0.7,0.7]
    red         = [1,0.0,0.0,1.0]
    lightRed    = [1,0.0,0.0,0.5]
    black       = [0,0,0,1.0]
    lightBlack  = [0,0,0,0.5]
    brown       = [0.85,0.75,0.15,1.0]
    lightBrown  = [0.85,0.75,0.15,1.0]
    
    def __init__(self):
        self.colors = (
                self.white       ,
                self.lightWhite  ,
                self.green       ,
                self.lightGreen  ,
                self.yellow      ,
                self.lightYellow ,
                self.blue        ,
                self.lightBlue   ,
                self.grey        ,
                self.lightGrey   ,
                self.red         ,
                self.lightRed    ,
                self.black       ,
                self.lightBlack  ,
                self.brown       ,
                self.lightBrown  ,
                )

    def __getitem__ (self, i):
        return self.colors[i]
