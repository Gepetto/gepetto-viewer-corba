# Copyright (c) 2014 CNRS
# Author: Mathieu Geisert, Florent Lamiraux
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

# Requires at least CMake 2.6 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CXX_DISABLE_WERROR true)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/boost.cmake)

SET(PROJECT_NAME gepetto-viewer-corba)
SET(PROJECT_DESCRIPTION "Corba server for gepetto-viewer")
SET(PROJECT_URL "")

SET(${PROJECT_NAME}_HEADERS
  include/gepetto/viewer/corba/windows-manager.hh
  include/gepetto/viewer/corba/server.hh
  include/gepetto/viewer/corba/client.hh
  include/gepetto/viewer/corba/fwd.hh
  )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

SETUP_PROJECT ()

# Dependencies
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")
ADD_REQUIRED_DEPENDENCY("openthreads >= 2.6")
ADD_REQUIRED_DEPENDENCY("gepetto-viewer")
ADD_REQUIRED_DEPENDENCY("urdfdom")
SEARCH_FOR_BOOST()

PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIRS})

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(blender)
ADD_SUBDIRECTORY(examples EXCLUDE_FROM_ALL)

SETUP_PROJECT_FINALIZE()
