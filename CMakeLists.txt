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

SET(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake-modules/
  ${CMAKE_MODULE_PATH})

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/boost.cmake)

SET(PROJECT_NAME gepetto-viewer-corba)
SET(PROJECT_DESCRIPTION "Corba server for gepetto-viewer")
SET(PROJECT_URL "")

SET(CLIENT_ONLY FALSE CACHE BOOL "Set to true to install the client only")

SET(${PROJECT_NAME}_HEADERS
  include/gepetto/viewer/corba/windows-manager.hh
  include/gepetto/viewer/corba/server.hh
  include/gepetto/viewer/corba/client.hh
  include/gepetto/viewer/corba/fwd.hh
  )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/bin)

OPTION(DISABLE_SAMPLING "Activate this option if your computer have no graphic card or a small one" OFF)
if(DISABLE_SAMPLING)
  ADD_DEFINITIONS(-DDISABLE_SAMPLING)
endif()

SETUP_PROJECT ()

# Dependencies
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")
IF(NOT CLIENT_ONLY)
  ADD_REQUIRED_DEPENDENCY("openscenegraph >= 3.2")
  ADD_REQUIRED_DEPENDENCY("openscenegraph-osgQt")
  ADD_REQUIRED_DEPENDENCY("openthreads >= 2.6")
  ADD_REQUIRED_DEPENDENCY("gepetto-viewer")
  ADD_REQUIRED_DEPENDENCY("urdfdom")

  SET(BOOST_COMPONENTS system thread regex)
  SEARCH_FOR_BOOST ()

  FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl QtNetwork)
  FINDPYTHON(2.7 EXACT)
  FIND_PACKAGE(PythonQt COMPONENTS QtAll)

  SET(GEPETTO_GUI_HAS_PYTHONQT ${PYTHONQT_FOUND} CACHE BOOL "Use PythonQt dependency")

  PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

  INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIRS})

  SET (${PROJECT_NAME}_HEADERS_MOC
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/mainwindow.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/safeapplication.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/osgwidget.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/tree-item.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/bodytreewidget.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/ledindicator.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/omniorb/omniorbthread.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/dialog/dialogloadrobot.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/dialog/dialogloadenvironment.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/dialog/pluginmanagerdialog.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/pick-handler.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/shortcut-factory.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/selection-handler.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/selection-event.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/windows-manager.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/action-search-bar.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/node-action.hh
    )
  IF(GEPETTO_GUI_HAS_PYTHONQT)
    SET (${PROJECT_NAME}_HEADERS_MOC
      ${${PROJECT_NAME}_HEADERS_MOC}
      ${CMAKE_SOURCE_DIR}/include/gepetto/gui/pythonwidget.hh)
  ENDIF(GEPETTO_GUI_HAS_PYTHONQT)
  SET (${PROJECT_NAME}_HEADERS_NO_MOC
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/fwd.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/meta.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/settings.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/color-map.hh
    ${CMAKE_BINARY_DIR}/include/gepetto/gui/config-dep.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/plugin-interface.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/omniorb/url.hh
    )

  SET(${PROJECT_NAME}_FORMS
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/ui/mainwindow.ui
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/ui/dialogloadrobot.ui
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/ui/dialogloadenvironment.ui
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/ui/pluginmanagerdialog.ui
    )

  SET(${PROJECT_NAME}_RESOURCES ${CMAKE_SOURCE_DIR}/res/images.qrc)

  SET(${PROJECT_NAME}_HEADERS
    ${${PROJECT_NAME}_HEADERS}
    ${${PROJECT_NAME}_HEADERS_MOC}
    ${${PROJECT_NAME}_HEADERS_NO_MOC})

  config_files(
    include/gepetto/gui/config-dep.hh
    src/gui/main.cc
    )
ENDIF(NOT CLIENT_ONLY)

ADD_SUBDIRECTORY(src)

IF(NOT CLIENT_ONLY)
  ADD_SUBDIRECTORY(src/gui)
  ADD_SUBDIRECTORY(plugins)
  ADD_SUBDIRECTORY(pyplugins)
  ADD_SUBDIRECTORY(blender)
  ADD_SUBDIRECTORY(examples EXCLUDE_FROM_ALL)
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT CLIENT_ONLY)

SETUP_PROJECT_FINALIZE()
