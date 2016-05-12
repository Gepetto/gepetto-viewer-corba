#
# Copyright (c) 2015 CNRS
# Authors: Joseph Mirabel
#
#
# This file is part of gepetto-gui
# gepetto-gui is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# gepetto-gui is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# gepetto-gui  If not, see
# <http://www.gnu.org/licenses/>.

CMAKE_MINIMUM_REQUIRED(VERSION 2.6)

SET(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake-modules/
  ${CMAKE_MODULE_PATH})

FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl QtNetwork)
FINDPYTHON(2.7 EXACT)
FIND_PACKAGE(PythonQt COMPONENTS QtAll)

ADD_REQUIRED_DEPENDENCY("openscenegraph >= 3.2")
ADD_REQUIRED_DEPENDENCY("openscenegraph-osgQt")
ADD_REQUIRED_DEPENDENCY("openthreads >= 2.6")
ADD_REQUIRED_DEPENDENCY("gepetto-viewer-corba")

SET(BOOST_COMPONENTS program_options system thread regex)
SEARCH_FOR_BOOST ()

FIND_PACKAGE("X11")

# Activate hpp-util logging if requested
SET (HPP_DEBUG FALSE CACHE BOOL "trigger hpp-util debug output")
IF (HPP_DEBUG)
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DHPP_DEBUG")
ENDIF()

# Configure the project

SET (${PROJECT_NAME}_SOURCES
  ${CMAKE_BINARY_DIR}/src/gui/main.cc
  settings.cc
  windows-manager.cc
  safeapplication.cc
  mainwindow.cc
  osgwidget.cc
  pick-handler.cc
  tree-item.cc
  omniorbthread.cc
  bodytreewidget.cc
  ledindicator.cc
  dialog/dialogloadrobot.cc
  dialog/dialogloadenvironment.cc
  dialog/pluginmanagerdialog.cc
  )

SET(GEPETTO_GUI_HAS_PYTHONQT ${PYTHONQT_FOUND} CACHE BOOL "Use PythonQt dependency")

IF(GEPETTO_GUI_HAS_PYTHONQT)
  INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIR}" "${PYTHONQT_INCLUDE_DIR}")

  SET (${PROJECT_NAME}_HEADERS_MOC
    ${${PROJECT_NAME}_HEADERS_MOC}
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/pythonwidget.hh)
  SET (${PROJECT_NAME}_SOURCES
    ${${PROJECT_NAME}_SOURCES}
    pythonwidget.cc)
ELSE (GEPETTO_GUI_HAS_PYTHONQT)
  MESSAGE(STATUS "Skipping PythonQt settings")
ENDIF(GEPETTO_GUI_HAS_PYTHONQT)

SET (${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS_MOC}
  ${${PROJECT_NAME}_HEADERS_NO_MOC}
  )

# Compile meta-objects and executable

QT4_WRAP_CPP(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_HEADERS_MOC})
QT4_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
QT4_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})

INCLUDE(${QT_USE_FILE})
ADD_DEFINITIONS(${QT_DEFINITIONS})

ADD_EXECUTABLE(gepetto-gui ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS_MOCED}
  ${${PROJECT_NAME}_HEADERS_NO_MOC}
  ${${PROJECT_NAME}_FORMS_HEADERS}
  ${${PROJECT_NAME}_RESOURCES_RCC}
  )

INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src/gui)

TARGET_LINK_LIBRARIES(gepetto-gui ${QT_LIBRARIES})
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openscenegraph)
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openthreads)
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui gepetto-viewer-corba)
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openscenegraph-osgQt)
IF (GEPETTO_GUI_HAS_PYTHONQT)
  TARGET_LINK_LIBRARIES(gepetto-gui ${PYTHONQT_LIBRARIES})
ENDIF (GEPETTO_GUI_HAS_PYTHONQT)
TARGET_LINK_LIBRARIES(gepetto-gui ${X11_LIBRARIES} ${Boost_LIBRARIES})

INSTALL(TARGETS gepetto-gui DESTINATION lib)

INSTALL(FILES ${CMAKE_BINARY_DIR}/src/gui/gepetto-gui
  DESTINATION bin
  PERMISSIONS OWNER_READ OWNER_WRITE OWNER_EXECUTE GROUP_READ GROUP_EXECUTE WORLD_READ WORLD_EXECUTE
  )
