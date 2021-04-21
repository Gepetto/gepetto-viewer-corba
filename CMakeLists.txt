# Copyright (c) 2014, 2020 CNRS
# Author: Mathieu Geisert, Florent Lamiraux, Guilhem Saurel
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

CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

SET(PROJECT_NAME gepetto-viewer-corba)
SET(PROJECT_DESCRIPTION "Corba server for gepetto-viewer")
SET(PROJECT_URL "https://github.com/Gepetto/${PROJECT_NAME}")

SET(PROJECT_USE_CMAKE_EXPORT TRUE)
SET(PROJECT_USE_KEYWORD_LINK_LIBRARIES TRUE)
SET(CXX_DISABLE_WERROR true)
SET(DOXYGEN_USE_TEMPLATE_CSS TRUE)

INCLUDE(cmake/base.cmake)
INCLUDE(cmake/idl.cmake)
INCLUDE(cmake/python.cmake)
INCLUDE(cmake/apple.cmake)

SET(CLIENT_ONLY FALSE CACHE BOOL "Set to true to install the client only")

COMPUTE_PROJECT_ARGS(PROJECT_ARGS LANGUAGES CXX)
PROJECT(${PROJECT_NAME} ${PROJECT_ARGS})

IF(APPLE)
  APPLY_DEFAULT_APPLE_CONFIGURATION()
ENDIF(APPLE)

# {{{ C++ and Python client.
# Dependencies
FINDPYTHON()
ADD_REQUIRED_DEPENDENCY("omniORB4 >= 4.1.4")

SET(${PROJECT_NAME}_HEADERS
  include/gepetto/viewer/corba/api.hh
  include/gepetto/viewer/corba/client.hh
  include/gepetto/viewer/corba/conversions.hh
  )
# }}}

# {{{ C++ server.
IF(NOT CLIENT_ONLY)
  # {{{ Dependencies for the server.
  ADD_PROJECT_DEPENDENCY("gepetto-viewer" REQUIRED)

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

  SET(CMAKE_INCLUDE_CURRENT_DIR ON)
  SET(GEPETTO_VIEWER_CORBA_QTVERSION "${GEPETTO_VIEWER_QTVERSION}")
  SET(PKG_CONFIG_EXTRA "qtversion=${GEPETTO_VIEWER_CORBA_QTVERSION}")
  SET(PACKAGE_EXTRA_MACROS "set(GEPETTO_VIEWER_CORBA_QTVERSION ${GEPETTO_VIEWER_CORBA_QTVERSION})")
  # }}}

  # {{{ Packaging
  PKG_CONFIG_APPEND_LIBS(${PROJECT_NAME})

  # For backward compatibility
  SET(PKG_CONFIG_EXTRA "${PKG_CONFIG_EXTRA}\ncmake_plugin=${GEPETTO_VIEWER_CMAKE_PLUGIN}")
  # }}}

  # {{{ Set list of headers
  LIST(APPEND ${PROJECT_NAME}_HEADERS
    ${CMAKE_SOURCE_DIR}/include/gepetto/gui/omniorb/url.hh
    )
  # }}}
ENDIF(NOT CLIENT_ONLY)
# }}}

ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(doc)

IF(NOT CLIENT_ONLY)
  ADD_SUBDIRECTORY(plugins)
  ADD_SUBDIRECTORY(blender)
  ADD_SUBDIRECTORY(examples EXCLUDE_FROM_ALL)
  ADD_SUBDIRECTORY(tests)
ENDIF(NOT CLIENT_ONLY)

INSTALL(FILES package.xml DESTINATION share/${PROJECT_NAME})
# vim: foldmethod=marker foldlevel=0
