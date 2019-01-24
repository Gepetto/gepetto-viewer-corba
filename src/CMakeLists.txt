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

ADD_CUSTOM_TARGET(generate_idl_cpp DEPENDS ${ALL_IDL_CPP_STUBS})
ADD_CUSTOM_TARGET(generate_idl_python DEPENDS ${ALL_IDL_PYTHON_STUBS})
# }}}

# {{{ C++ client: generate the library
ADD_LIBRARY(${LIBRARY_NAME}
  SHARED
  client.cc
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface.hh
  ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interfaceSK.cc
)
ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_cpp)
ADD_DEPENDENCIES (${LIBRARY_NAME} generate_idl_python)

PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} omniORB4)

INSTALL(TARGETS ${LIBRARY_NAME} DESTINATION lib)
# }}}

# {{{ C++ application: generate the executable if requested
IF(NOT CLIENT_ONLY)

  # {{{ Setup source files
  SET (${PROJECT_NAME}_SOURCES
    graphical-interface.impl.cpp
    server.cc
    server-private.cc
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interfaceSK.cc

    ${CMAKE_BINARY_DIR}/src/gui/main.cc
    gui/settings.cc
    gui/windows-manager.cc
    gui/safeapplication.cc
    gui/mainwindow.cc
    gui/osgwidget.cc
    gui/pick-handler.cc
    gui/tree-item.cc
    gui/omniorbthread.cc
    gui/bodytreewidget.cc
    gui/ledindicator.cc
    gui/dialog/dialogloadrobot.cc
    gui/dialog/dialogloadenvironment.cc
    gui/dialog/pluginmanagerdialog.cc
    gui/shortcut-factory.cc
    gui/selection-handler.cc
    gui/selection-event.cc
    gui/action-search-bar.cc
    gui/node-action.cc
    )

  # Removes 5 warnings about anonymous variadic macros that were added from
  # c++ 11.
  SET_SOURCE_FILES_PROPERTIES(graphical-interface.impl.cpp
    PROPERTIES
    COMPILE_FLAGS "-Wno-variadic-macros")
  # }}}

  # {{{ Configure PythonQt
  IF(GEPETTO_GUI_HAS_PYTHONQT)
    INCLUDE_DIRECTORIES(SYSTEM "${PYTHON_INCLUDE_DIR}" "${PYTHONQT_INCLUDE_DIR}")
    SET (${PROJECT_NAME}_SOURCES
      ${${PROJECT_NAME}_SOURCES}
      gui/pythonwidget.cc)
    SET (${PROJECT_NAME}_MOC
      ${${PROJECT_NAME}_HEADERS_MOC}
      gui/python-decorator.hh
      )
  ELSE (GEPETTO_GUI_HAS_PYTHONQT)
    MESSAGE(STATUS "Skipping PythonQt settings")
    SET (${PROJECT_NAME}_MOC
      ${${PROJECT_NAME}_HEADERS_MOC}
      )
  ENDIF(GEPETTO_GUI_HAS_PYTHONQT)
  # }}}

  # {{{ QT config: Compile meta-objects and executable
  SET (${PROJECT_NAME}_HEADERS
    ${${PROJECT_NAME}_HEADERS_MOC}
    ${${PROJECT_NAME}_HEADERS_NO_MOC}
    )

  IF(USE_QT4)
    QT4_WRAP_CPP(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_MOC})
    QT4_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
    QT4_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})
    INCLUDE(${QT_USE_FILE})
  ELSE(USE_QT4)
    QT5_WRAP_CPP(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_MOC})
    QT5_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
    QT5_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC ${${PROJECT_NAME}_RESOURCES})
  ENDIF(USE_QT4)

  ADD_DEFINITIONS(${QT_DEFINITIONS})
  # }}}

  # {{{ Compile, link and install
  ADD_EXECUTABLE(gepetto-gui ${${PROJECT_NAME}_SOURCES}
    ${${PROJECT_NAME}_HEADERS_MOCED}
    ${${PROJECT_NAME}_HEADERS_NO_MOC}
    ${${PROJECT_NAME}_FORMS_HEADERS}
    ${${PROJECT_NAME}_RESOURCES_RCC}
    )

  ADD_DEPENDENCIES (gepetto-gui generate_idl_cpp)
  ADD_DEPENDENCIES (gepetto-gui generate_idl_python)

  TARGET_LINK_LIBRARIES(gepetto-gui ${QT_LIBRARIES} ${Boost_LIBRARIES})
  PKG_CONFIG_USE_DEPENDENCY(gepetto-gui gepetto-viewer)
  PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openscenegraph)
  PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openthreads)
  PKG_CONFIG_USE_DEPENDENCY(gepetto-gui ${OSGQT_LIB_NAME})
  PKG_CONFIG_USE_DEPENDENCY(gepetto-gui omniORB4)
  IF (GEPETTO_GUI_HAS_PYTHONQT)
    TARGET_LINK_LIBRARIES(gepetto-gui ${PYTHONQT_LIBRARIES})
  ENDIF (GEPETTO_GUI_HAS_PYTHONQT)

  INSTALL(TARGETS gepetto-gui DESTINATION bin)

  #PKG_CONFIG_USE_DEPENDENCY(${LIBRARY_NAME} urdfdom)

  # }}}

ENDIF(NOT CLIENT_ONLY)
# }}}

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
