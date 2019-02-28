# Copyright (c) 2014-2016 LAAS-CNRS
# Author: Mathieu Geisert
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

# {{{ Generate C++ and Python files from IDL files.
SET(IDL_SOURCES
  graphical-interface
  )

OMNIIDL_INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/idl)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

FOREACH(IDL ${IDL_SOURCES})
  # C++
  GENERATE_IDL_CPP (gepetto/viewer/corba/${IDL}
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer)
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gepetto/viewer/corba)

  # Python
  GENERATE_IDL_PYTHON (gepetto/viewer/corba/${IDL}
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer
    ARGUMENTS
    -p${CMAKE_SOURCE_DIR}/cmake/hpp/idl
    -bomniidl_be_python_with_docstring
    -C${CMAKE_CURRENT_BINARY_DIR}
    -K
    -Wbpackage=gepetto.corbaserver
    )

  STRING(REGEX REPLACE "-" "_" IDL_UNDERSCORE "${IDL}")

  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/gepetto/corbaserver/${IDL_UNDERSCORE}_idl.py
    DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver)
  INSTALL(DIRECTORY
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/corbaserver/gepetto
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/corbaserver/gepetto__POA
    DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver)

  # IDL
  INSTALL(FILES ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer/${IDL}.idl
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idl/gepetto/corbaserver)
ENDFOREACH()

ADD_CUSTOM_TARGET(generate_idl_cpp DEPENDS ${ALL_IDL_CPP_STUBS})
ADD_CUSTOM_TARGET(generate_idl_python DEPENDS ${ALL_IDL_PYTHON_STUBS})
# }}}

# {{{ C++ client: generate the library
SET (${PROJECT_NAME}_SOURCES
  client.cc
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface.hh
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interfaceSK.cc
)
# }}}

# {{{ C++ application: generate the executable if requested
IF(NOT CLIENT_ONLY)
  # {{{ Configure Qt
  SET(${PROJECT_NAME}_MOC
    node-callback.hh)
  IF(PROJECT_USE_QT4)
    QT4_WRAP_CPP(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_MOC})
    #QT4_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
    #QT4_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})
    INCLUDE(${QT_USE_FILE})
  ELSE(PROJECT_USE_QT4)
    QT5_WRAP_CPP(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_MOC})
    #QT5_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
    #QT5_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})
  ENDIF(PROJECT_USE_QT4)

  # {{{ Setup source files
  SET (${PROJECT_NAME}_SOURCES
    ${${PROJECT_NAME}_SOURCES}
    graphical-interface.impl.cpp
    node-callback.cpp
    server.cc
    server-private.cc
    ${${PROJECT_NAME}_HEADERS_MOCED}
    )

ADD_DEFINITIONS(${QT_DEFINITIONS})
# }}}

  # Removes 5 warnings about anonymous variadic macros that were added from
  # c++ 11.
  SET_SOURCE_FILES_PROPERTIES(graphical-interface.impl.cpp
    PROPERTIES
    COMPILE_FLAGS "-Wno-variadic-macros")
  # }}}

ENDIF(NOT CLIENT_ONLY)
# }}}

ADD_LIBRARY(${LIBRARY_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_cpp)
ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_python)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} gepetto-viewer)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} omniORB4)

INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION lib)

# {{{ Python client: install Python files
MAKE_DIRECTORY(${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba)

INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/__init__.py
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/quaternion.py
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/color.py
  DESTINATION ${PYTHON_SITELIB}/gepetto
  )
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/__init__.py
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/client.py
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/exporttoblender.py
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/tools.py
  DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver
  )
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/../blender/gepettoimport.py
  DESTINATION ${PYTHON_SITELIB}/gepetto/blender
  )
# }}}

# vim: foldmethod=marker foldlevel=0
