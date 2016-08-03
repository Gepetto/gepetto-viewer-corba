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
SET(IDL_SOURCES
  graphical-interface
  )

# search for python
FINDPYTHON(2.7 EXACT REQUIRED)

OMNIIDL_INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/idl)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)

FOREACH(IDL ${IDL_SOURCES})
  GENERATE_IDL_CPP (gepetto/viewer/corba/${IDL}
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer)
  GENERATE_IDL_PYTHON (gepetto/viewer/corba/${IDL}
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer)

  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gepetto/viewer/corba)

  STRING(REGEX REPLACE "-" "_" IDL_UNDERSCORE "${IDL}")
  IF(NOT ${IDL} EQUAL ${IDL_UNDERSCORE})
    ADD_CUSTOM_COMMAND(OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL_UNDERSCORE}_idl.py
      COMMAND ${CMAKE_COMMAND}
      ARGS -E copy ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}_idl.py
      )
  ENDIF(NOT ${IDL} EQUAL ${IDL_UNDERSCORE})

  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL_UNDERSCORE}_idl.py
    DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver)

  INSTALL(FILES ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer/${IDL}.idl
    DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idl/gepetto/corbaserver)
ENDFOREACH()

IF(NOT CLIENT_ONLY)

  ADD_CUSTOM_TARGET(generate_idl_cpp DEPENDS ${ALL_IDL_CPP_STUBS})
  ADD_CUSTOM_TARGET(generate_idl_python DEPENDS ${ALL_IDL_PYTHON_STUBS})

  ADD_LIBRARY(${LIBRARY_NAME}
    SHARED
    graphical-interface.impl.cpp
    graphical-interface.impl.hh
    windows-manager.cpp
    server.cc
    client.cc
    server-private.cc
    server-private.hh
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface.hh
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interfaceSK.cc
  )

  ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_cpp)
  ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_python)

  PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} omniORB4)
  PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} gepetto-viewer)
  PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} urdfdom)
  TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${Boost_LIBRARIES})

  INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION lib)

  # Standalone corba server
  ADD_EXECUTABLE (gepetto-viewer-server corbaserver.cc)
  TARGET_LINK_LIBRARIES(gepetto-viewer-server ${LIBRARY_NAME})
  PKG_CONFIG_USE_DEPENDENCY(gepetto-viewer-server omniORB4)
  #TARGET_LINK_LIBRARIES(gepetto-viewer-server ${Boost_LIBRARIES})

  INSTALL (TARGETS gepetto-viewer-server DESTINATION bin)

ELSE(NOT CLIENT_ONLY)

  ADD_CUSTOM_TARGET(generate_idl_cpp ALL DEPENDS ${ALL_IDL_CPP_STUBS})
  ADD_CUSTOM_TARGET(generate_idl_python ALL DEPENDS ${ALL_IDL_PYTHON_STUBS})

ENDIF(NOT CLIENT_ONLY)

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
  DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver
  )
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/client.py
  DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver
  )
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/exporttoblender.py
  DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver
  )
INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/../blender/gepettoimport.py
  DESTINATION ${PYTHON_SITELIB}/gepetto/blender
  )
