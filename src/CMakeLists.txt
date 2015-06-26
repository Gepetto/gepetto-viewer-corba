# Copyright (c) 2014 LAAS-CNRS
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

OMNIIDL_INCLUDE_DIRECTORIES(${CMAKE_SOURCE_DIR}/idl)
INCLUDE_DIRECTORIES(${CMAKE_BINARY_DIR}/src)
FOREACH(IDL ${IDL_SOURCES})
  GENERATE_IDL_FILE (gepetto/viewer/corba/${IDL}
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer)
ENDFOREACH()

ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  graphical-interface.impl.cpp
  graphical-interface.impl.hh
  windows-manager.cpp
  server.cc
  client.cc
  server-private.cc
  server-private.hh
  client-cpp.cc
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface_idl.py
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface.hh
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interfaceSK.cc
)

PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} omniORB4)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} gepetto-viewer)
PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} urdfdom)
TARGET_LINK_LIBRARIES(${LIBRARY_NAME} ${Boost_LIBRARIES})

INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION lib)

# search for python
FINDPYTHON()

MAKE_DIRECTORY(${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba)

FOREACH (IDL ${IDL_SOURCES})
  ADD_CUSTOM_COMMAND(
    OUTPUT
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}_idl.py
    COMMAND ${OMNIIDL}
    ARGS -bpython -Wbpackage="" -I${CMAKE_SOURCE_DIR}/idl
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer/${IDL}.idl
    MAIN_DEPENDENCY
    ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer/${IDL}.idl
    )
  SET_SOURCE_FILES_PROPERTIES(
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}_idl.py
    PROPERTIES GENERATED ON
    )
  INSTALL(FILES ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}.hh
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gepetto/viewer/corba)

  STRING(REGEX REPLACE "-" "_"
    IDL_UNDERSCORE "${IDL}")

  INSTALL(
    FILES
    ${CMAKE_CURRENT_BINARY_DIR}/${IDL_UNDERSCORE}_idl.py
    DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver
    )
ENDFOREACH()

INSTALL(
  FILES
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/__init__.py
  ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/quaternion.py
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
  ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer/graphical-interface.idl
  DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idl/gepetto/corbaserver
)

INSTALL (FILES
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface.hh
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gepetto/viewer/corba
  )

# Standalone corba server
ADD_EXECUTABLE (gepetto-viewer-server corbaserver.cc)
TARGET_LINK_LIBRARIES(gepetto-viewer-server ${LIBRARY_NAME})
#TARGET_LINK_LIBRARIES(gepetto-viewer-server ${Boost_LIBRARIES})

ADD_EXECUTABLE (client-test test-client-cpp.cc)
TARGET_LINK_LIBRARIES(client-test ${LIBRARY_NAME})
PKG_CONFIG_USE_DEPENDENCY(client-test gepetto-viewer)

INSTALL (TARGETS gepetto-viewer-server DESTINATION bin)
