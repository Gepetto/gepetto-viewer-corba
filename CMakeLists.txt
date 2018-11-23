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

# Requires at least CMake 2.8 to configure the package.
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

SET(CXX_DISABLE_WERROR true)

SET(CMAKE_MODULE_PATH
  ${CMAKE_SOURCE_DIR}/cmake-modules/
  ${CMAKE_MODULE_PATH})

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/test.cmake)

SET(PROJECT_NAME gepetto-viewer-corba)
SET(PROJECT_DESCRIPTION "Corba server for gepetto-viewer")
SET(PROJECT_URL "https://github.com/Gepetto/gepetto-viewer-corba")

SET(CLIENT_ONLY FALSE CACHE BOOL "Set to true to install the client only")
SET(USE_QT4 TRUE CACHE BOOL "Use Qt4 instead of Qt5")

SET(${PROJECT_NAME}_HEADERS
  include/gepetto/viewer/corba/server.hh
  include/gepetto/viewer/corba/client.hh
  include/gepetto/viewer/corba/fwd.hh
  )

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

OPTION(DISABLE_SAMPLING "Activate this option if your computer have no graphic card or a small one" OFF)
if(DISABLE_SAMPLING)
  ADD_DEFINITIONS(-DDISABLE_SAMPLING)
endif()

SETUP_PROJECT ()

# Dependencies
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")

IF(NOT CLIENT_ONLY)
  ADD_REQUIRED_DEPENDENCY("openscenegraph >= 3.2")

  IF(USE_QT4 OR OPENSCENEGRAPH_VERSION VERSION_LESS 3.5.7)
    SET(OSGQT_LIB_NAME openscenegraph-osgQt)
  ELSE(USE_QT4 OR OPENSCENEGRAPH_VERSION VERSION_LESS 3.5.7)
    SET(OSGQT_LIB_NAME openscenegraph-osgQt5)
  ENDIF(USE_QT4 OR OPENSCENEGRAPH_VERSION VERSION_LESS 3.5.7)

  ADD_REQUIRED_DEPENDENCY(${OSGQT_LIB_NAME})
  ADD_REQUIRED_DEPENDENCY("openthreads >= 2.6")
  ADD_REQUIRED_DEPENDENCY("gepetto-viewer")
  ADD_REQUIRED_DEPENDENCY("urdfdom")

  SET(BOOST_COMPONENTS system regex)
  SEARCH_FOR_BOOST ()
  FINDPYTHON()

  SET(CMAKE_INCLUDE_CURRENT_DIR ON)
  IF(USE_QT4)
    FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtOpenGl QtNetwork)
    SET(PKG_CONFIG_EXTRA "qtversion=${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
  ELSE(USE_QT4)
    FOREACH (component "Core" "Widgets" "Gui" "OpenGL" "Network")
      FIND_PACKAGE ("Qt5${component}" REQUIRED)
      LIST(APPEND QT_INCLUDE_DIRS ${Qt5${component}_INCLUDE_DIRS})
      LIST(APPEND QT_LIBRARIES    ${Qt5${component}_LIBRARIES}   )
    ENDFOREACH (component "Core" "Widgets" "Gui" "OpenGL" "Network")
    SET(PKG_CONFIG_EXTRA "qtversion=${Qt5Core_VERSION}")
  ENDIF(USE_QT4)

  # If GEPETTO_GUI_HAS_PYTHONQT is set to TRUE, then PythonQt is required.
  IF(DEFINED GEPETTO_GUI_HAS_PYTHONQT AND GEPETTO_GUI_HAS_PYTHONQT)
    FIND_PACKAGE(PythonQt REQUIRED COMPONENTS QtAll)
  ELSE()
    FIND_PACKAGE(PythonQt COMPONENTS QtAll)
  ENDIF()
  # Declare option GEPETTO_GUI_HAS_PYTHONQT
  # If GEPETTO_GUI_HAS_PYTHONQT was defined, this does not change its value.
  IF (PythonQt_FOUND AND PythonQt_QtAll_FOUND)
    SET(GEPETTO_GUI_HAS_PYTHONQT TRUE CACHE BOOL "Use PythonQt dependency")
  ELSE()
    SET(GEPETTO_GUI_HAS_PYTHONQT FALSE CACHE BOOL "Use PythonQt dependency")
  ENDIF()

  PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

  INSTALL(FILES cmake-modules/gepetto-viewer-corba-plugin.cmake
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake/
    PERMISSIONS OWNER_READ GROUP_READ WORLD_READ OWNER_WRITE)
  SET(PKG_CONFIG_EXTRA "${PKG_CONFIG_EXTRA}\ncmake_plugin=${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_DATAROOTDIR}/${PROJECT_NAME}/cmake/gepetto-viewer-corba-plugin.cmake")

  INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIRS} ${QT_INCLUDE_DIRS})

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
