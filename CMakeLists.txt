# Copyright (c) 2012 CNRS
# Author: Florent Lamiraux
#
# This file is part of hpp-corbaserver.
# hpp-corbaserver is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# hpp-corbaserver is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

# Requires at least CMake 2.6 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CXX_DISABLE_WERROR true)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/cpack.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/boost.cmake)

SET(PROJECT_NAME SceneViewer-corba)
SET(PROJECT_DESCRIPTION "Corba server for SceneViewer")
SET(PROJECT_URL "")

SET(${PROJECT_NAME}_HEADERS
  src/client-cpp.hh
  )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SETUP_PROJECT ()

# Dependencies
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")
ADD_REQUIRED_DEPENDENCY("openscenegraph >= 3.2")
ADD_REQUIRED_DEPENDENCY("openthreads >= 3.2")
ADD_REQUIRED_DEPENDENCY("SceneViewer")
ADD_REQUIRED_DEPENDENCY("urdfdom")
ADD_REQUIRED_DEPENDENCY("eigen3 >= v3.0.5")
SEARCH_FOR_BOOST()

INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIRS})

ADD_SUBDIRECTORY(src)

SETUP_PROJECT_FINALIZE()
#SETUP_PROJECT_CPACK()
