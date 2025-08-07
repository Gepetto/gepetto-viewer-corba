# Copyright (c) 2014, 2020 CNRS Author: Mathieu Geisert, Florent Lamiraux,
# Guilhem Saurel
#
# This file is part of gepetto-viewer-corba. gepetto-viewer-corba is free
# software: you can redistribute it and/or modify it under the terms of the GNU
# Lesser General Public License as published by the Free Software Foundation,
# either version 3 of the License, or (at your option) any later version.
#
# gepetto-viewer-corba is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License
# for more details.  You should have received a copy of the GNU Lesser General
# Public License along with gepetto-viewer-corba.  If not, see
# <http://www.gnu.org/licenses/>.

cmake_minimum_required(VERSION 3.22)

set(PROJECT_NAME gepetto-viewer-corba)
set(PROJECT_DESCRIPTION "Corba server for gepetto-viewer")
set(PROJECT_URL "https://github.com/Gepetto/${PROJECT_NAME}")

set(PROJECT_USE_CMAKE_EXPORT TRUE)
set(PROJECT_USE_KEYWORD_LINK_LIBRARIES TRUE)
set(CXX_DISABLE_WERROR true)
set(DOXYGEN_USE_TEMPLATE_CSS TRUE)

# Check if the submodule cmake have been initialized
set(JRL_CMAKE_MODULES "${CMAKE_CURRENT_LIST_DIR}/cmake")
if(EXISTS "${JRL_CMAKE_MODULES}/base.cmake")
  message(STATUS "JRL cmakemodules found in 'cmake/' git submodule")
else()
  find_package(jrl-cmakemodules QUIET CONFIG)
  if(jrl-cmakemodules_FOUND)
    get_property(
      JRL_CMAKE_MODULES
      TARGET jrl-cmakemodules::jrl-cmakemodules
      PROPERTY INTERFACE_INCLUDE_DIRECTORIES)
    message(STATUS "JRL cmakemodules found on system at ${JRL_CMAKE_MODULES}")
  elseif(${CMAKE_VERSION} VERSION_LESS "3.14.0")
    message(
      FATAL_ERROR
        "\nCan't find jrl-cmakemodules. Please either:\n"
        "  - use git submodule: 'git submodule update --init'\n"
        "  - or install https://github.com/jrl-umi3218/jrl-cmakemodules\n"
        "  - or upgrade your CMake version to >= 3.14 to allow automatic fetching\n"
    )
  else()
    message(STATUS "JRL cmakemodules not found. Let's fetch it.")
    include(FetchContent)
    FetchContent_Declare(
      "jrl-cmakemodules"
      GIT_REPOSITORY "https://github.com/jrl-umi3218/jrl-cmakemodules.git")
    FetchContent_MakeAvailable("jrl-cmakemodules")
    FetchContent_GetProperties("jrl-cmakemodules" SOURCE_DIR JRL_CMAKE_MODULES)
  endif()
endif()

include("${JRL_CMAKE_MODULES}/base.cmake")
include("${JRL_CMAKE_MODULES}/idl.cmake")
include("${JRL_CMAKE_MODULES}/apple.cmake")

set(CLIENT_ONLY
    FALSE
    CACHE BOOL "Set to true to install the client only")

compute_project_args(PROJECT_ARGS LANGUAGES CXX)
project(${PROJECT_NAME} ${PROJECT_ARGS})

if(APPLE)
  apply_default_apple_configuration()
endif(APPLE)

# {{{ C++ and Python client. Dependencies
add_required_dependency("omniORB4 >= 4.1.4")

set(${PROJECT_NAME}_HEADERS
    include/gepetto/viewer/corba/api.hh include/gepetto/viewer/corba/client.hh
    include/gepetto/viewer/corba/conversions.hh)
# }}}

# {{{ C++ server.
if(NOT CLIENT_ONLY)
  # {{{ Dependencies for the server.
  add_project_dependency("gepetto-viewer" REQUIRED)

  # Get desired Qt version
  string(REPLACE "." ";" DESIRED_QT_VERSION_LIST ${GEPETTO_VIEWER_QTVERSION})
  list(GET DESIRED_QT_VERSION_LIST 0 DESIRED_QT_VERSION_MAJOR)

  if(${DESIRED_QT_VERSION_MAJOR} EQUAL 4)
    set(PROJECT_USE_QT4 True)
  elseif(${DESIRED_QT_VERSION_MAJOR} EQUAL 5)
    set(PROJECT_USE_QT4 False)
  else()
    message(FATAL_ERROR "This package is only compatible with Qt 4 and Qt 5")
  endif()

  set(CMAKE_INCLUDE_CURRENT_DIR ON)
  set(GEPETTO_VIEWER_CORBA_QTVERSION "${GEPETTO_VIEWER_QTVERSION}")
  set(PKG_CONFIG_EXTRA "qtversion=${GEPETTO_VIEWER_CORBA_QTVERSION}")
  set(PACKAGE_EXTRA_MACROS
      "set(GEPETTO_VIEWER_CORBA_QTVERSION ${GEPETTO_VIEWER_CORBA_QTVERSION})")
  # }}}

  # {{{ Packaging
  pkg_config_append_libs(${PROJECT_NAME})

  # For backward compatibility
  set(PKG_CONFIG_EXTRA
      "${PKG_CONFIG_EXTRA}\ncmake_plugin=${GEPETTO_VIEWER_CMAKE_PLUGIN}")
  # }}}

  # {{{ Set list of headers
  list(APPEND ${PROJECT_NAME}_HEADERS
       ${CMAKE_SOURCE_DIR}/include/gepetto/gui/omniorb/url.hh)
  # }}}
endif(NOT CLIENT_ONLY)
# }}}

add_subdirectory(src)
add_subdirectory(doc)

if(NOT CLIENT_ONLY)
  add_subdirectory(plugins)
  add_subdirectory(blender)
  add_subdirectory(examples EXCLUDE_FROM_ALL)
  add_subdirectory(tests)
endif(NOT CLIENT_ONLY)

install(FILES package.xml DESTINATION share/${PROJECT_NAME})
# vim: foldmethod=marker foldlevel=0
