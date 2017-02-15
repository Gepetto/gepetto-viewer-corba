# Copyright 2016 CNRS-LAAS
#
# Author: Joseph Mirabel
#
# This file is part of hpp-gui
# hpp-gui is free software: you can redistribute it and/or modify
# it under the terms of the GNU Lesser General Public License as published by
# the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# hpp-gui is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Lesser Public License for more details.
# You should have received a copy of the GNU Lesser General Public License
# along with hpp-gui  If not, see <http://www.gnu.org/licenses/>.

# ADD_TESTCASE(NAME)
# ------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against
# Boost and add it to the test suite.
#

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

MACRO(ADD_TESTCASE NAME GENERATED)
  IF (${GENERATED} STREQUAL TRUE)
    ADD_EXECUTABLE(${NAME} ${CMAKE_CURRENT_BINARY_DIR}/${NAME}.cc)
  ELSE()
    ADD_EXECUTABLE(${NAME} ${NAME}.cc)
  ENDIF()
  ADD_TEST(${NAME} ${EXECUTABLE_OUTPUT_PATH}/${NAME})

  TARGET_LINK_LIBRARIES(${NAME} ${QT_LIBRARIES})
  PKG_CONFIG_USE_DEPENDENCY(${NAME} openscenegraph)
  PKG_CONFIG_USE_DEPENDENCY(${NAME} openthreads)
  PKG_CONFIG_USE_DEPENDENCY(${NAME} openscenegraph-osgQt)
  PKG_CONFIG_USE_DEPENDENCY(${NAME} gepetto-viewer)

ENDMACRO(ADD_TESTCASE)

ADD_TESTCASE (colormap FALSE)
# ADD_TESTCASE (osgviewerQt FALSE)
