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

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/boost.cmake)
INCLUDE(cmake/test.cmake)

SET(PROJECT_NAME gepetto-viewer-corba)
SET(PROJECT_DESCRIPTION "Corba server for gepetto-viewer")
SET(PROJECT_URL "https://github.com/Gepetto/${PROJECT_NAME}")

SET(CLIENT_ONLY FALSE CACHE BOOL "Set to true to install the client only")

SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/lib)

SETUP_PROJECT ()

# {{{ C++ and Python client.
# Dependencies
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")

SET(${PROJECT_NAME}_HEADERS
  include/gepetto/viewer/corba/client.hh
  )
# }}}

# {{{ C++ server.
IF(NOT CLIENT_ONLY)
  # {{{ Dependencies for the server.
  FINDPYTHON()

  # Tells pkg-config to read qtversion and cmake_plugin from pkg config file.
  LIST(APPEND PKG_CONFIG_ADDITIONAL_VARIABLES qtversion cmake_plugin)
  ADD_REQUIRED_DEPENDENCY("gepetto-viewer > 4.3.0")

  # Get desired Qt version
  string(REPLACE "." ";" DESIRED_QT_VERSION_LIST ${GEPETTO_VIEWER_QTVERSION})
  list(GET DESIRED_QT_VERSION_LIST 0 DESIRED_QT_VERSION_MAJOR)

  IF(${DESIRED_QT_VERSION_MAJOR} EQUAL 4)
    SET(PROJECT_USE_QT4 True)
  ELSEIF(${DESIRED_QT_VERSION_MAJOR} EQUAL 5)
    SET(PROJECT_USE_QT4 False)
  ELSE()
    MESSAGE(FATAL_ERROR "This package is only compatible with Qt 4 and Qt 5")
  ENDIF()
  MESSAGE(STATUS "Looking for Qt ${DESIRED_QT_VERSION_MAJOR}.")

  SET(CMAKE_INCLUDE_CURRENT_DIR ON)
  IF(PROJECT_USE_QT4)
    FIND_PACKAGE(Qt4 REQUIRED QtCore QtGui QtNetwork)
    SET(PKG_CONFIG_EXTRA "qtversion=${QT_VERSION_MAJOR}.${QT_VERSION_MINOR}.${QT_VERSION_PATCH}")
    INCLUDE(${QT_USE_FILE})
  ELSE(PROJECT_USE_QT4)
    FOREACH (component "Core" "Widgets" "Gui" "Network" "Concurrent")
      FIND_PACKAGE ("Qt5${component}" REQUIRED)
      LIST(APPEND QT_INCLUDE_DIRS ${Qt5${component}_INCLUDE_DIRS})
      LIST(APPEND QT_LIBRARIES    ${Qt5${component}_LIBRARIES}   )
    ENDFOREACH (component "Core" "Widgets" "Gui" "OpenGL" "Network")
    SET(PKG_CONFIG_EXTRA "qtversion=${Qt5Core_VERSION}")
  ENDIF(PROJECT_USE_QT4)

  INCLUDE_DIRECTORIES(SYSTEM ${EIGEN3_INCLUDE_DIRS} ${QT_INCLUDE_DIRS})
  # }}}

  # {{{ PythonQt settings
  SET(CMAKE_MODULE_PATH
    ${CMAKE_SOURCE_DIR}/cmake-modules/
    ${CMAKE_MODULE_PATH})
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
  ADD_OPTIONAL_DEPENDENCY("qgv")
  # }}}

  # {{{ Packaging
  PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

  # For backward compatibility
  SET(PKG_CONFIG_EXTRA "${PKG_CONFIG_EXTRA}\ncmake_plugin=${GEPETTO_VIEWER_CMAKE_PLUGIN}")
  INCLUDE(${GEPETTO_VIEWER_CMAKE_PLUGIN})
  # }}}

  # {{{ Set list of headers
  SET (${PROJECT_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/corba/server.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/viewer/corba/fwd.hh
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/omniorb/url.hh
    )
  # }}}
ENDIF(NOT CLIENT_ONLY)
# }}}

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)

IF(NOT CLIENT_ONLY)
  ADD_SUBDIRECTORY(plugins)
  ADD_SUBDIRECTORY(pyplugins)
  ADD_SUBDIRECTORY(blender)
  ADD_SUBDIRECTORY(examples EXCLUDE_FROM_ALL)
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT CLIENT_ONLY)

SETUP_PROJECT_FINALIZE()

# vim: foldmethod=marker foldlevel=0
