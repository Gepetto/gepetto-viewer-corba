# Copyright (c) 2015 LAAS-CNRS
# Author: Joseph Mirabel
#
# This file is part of SceneViewer-corba.
# SceneViewer-corba is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# SceneViewer-corba is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

SET(LIBRARY_NAME ${PROJECT_NAME})

MACRO(ADD_EXAMPLE NAME SOURCE)
    ADD_EXECUTABLE (${NAME} ${SOURCE})
    TARGET_LINK_LIBRARIES(${NAME} ${LIBRARY_NAME})
    PKG_CONFIG_USE_DEPENDENCY(${NAME} gepetto-viewer)
    PKG_CONFIG_USE_DEPENDENCY(${NAME} omniORB4)
ENDMACRO()

ADD_EXAMPLE (basic-scene basic-scene.cc)
ADD_EXAMPLE (display-urdf display-urdf.cc)
ADD_EXAMPLE (free-functions free-functions.cc)
