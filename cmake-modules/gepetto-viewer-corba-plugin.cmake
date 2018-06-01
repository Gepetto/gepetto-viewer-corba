# Copyright (c) 2018 CNRS
# Authors: Joseph Mirabel
#
#
# This file is part of gepetto-viewer-corba
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
# gepetto-viewer-corba  If not, see
# <http://www.gnu.org/licenses/>.

# See plugins/pluginskeleton/CMakeLists.txt for description of how to use this macro.
MACRO(GEPETTO_GUI_PLUGIN PLUGIN_NAME)

  SET(options QT4 INSIDE_GEPETTO_VIEWER_CORBA)
  SET(oneValueArgs )
  SET(multiValueArgs
    HEADERS
    HEADERS_NO_MOC
    FORMS
    RESOURCES
    SOURCES
    LINK_DEPENDENCIES
    PKG_CONFIG_DEPENDENCIES)
  CMAKE_PARSE_ARGUMENTS(PLUGIN "${options}" "${oneValueArgs}" "${multiValueArgs}" ${ARGN})

  IF(PLUGIN_QT4)
    IF(PLUGIN_INSIDE_GEPETTO_VIEWER_CORBA)
      QT4_WRAP_CPP(PLUGIN_HEADERS_MOC ${PLUGIN_HEADERS})
    ELSE()
      QT4_WRAP_CPP(PLUGIN_HEADERS_MOC ${PLUGIN_HEADERS} OPTIONS -I${GEPETTO_VIEWER_CORBA_INCLUDEDIR})
    ENDIF()
    QT4_WRAP_UI(PLUGIN_FORMS_HEADERS ${PLUGIN_FORMS})
    QT4_ADD_RESOURCES(PLUGIN_RESOURCES_RCC ${PLUGIN_RESOURCES})
    INCLUDE(${QT_USE_FILE})
  ELSE()
    IF(PLUGIN_INSIDE_GEPETTO_VIEWER_CORBA)
      QT5_WRAP_CPP(PLUGIN_HEADERS_MOC ${PLUGIN_HEADERS})
    ELSE()
      QT5_WRAP_CPP(PLUGIN_HEADERS_MOC ${PLUGIN_HEADERS} OPTIONS -I${GEPETTO_VIEWER_CORBA_INCLUDEDIR})
    ENDIF()
    QT5_WRAP_UI(PLUGIN_FORMS_HEADERS ${PLUGIN_FORMS})
    QT5_ADD_RESOURCES(PLUGIN_RESOURCES_RCC ${PLUGIN_RESOURCES})
  ENDIF()


  INCLUDE_DIRECTORIES(${CMAKE_CURRENT_SOURCE_DIR})
  ADD_DEFINITIONS(${QT_DEFINITIONS}
                  -DQT_PLUGIN
                  -DQT_NO_DEBUG
                  -DQT_SHARED)

  IF(APPLE)
    SET(CMAKE_MACOSX_RPATH TRUE)
  ENDIF(APPLE)
  SET(CMAKE_INSTALL_RPATH "${CMAKE_INSTALL_PREFIX}/lib/gepetto-gui-plugins")

  ADD_LIBRARY(${PLUGIN_NAME} SHARED ${PLUGIN_SOURCES}
    ${PLUGIN_HEADERS_NO_MOC}
    ${PLUGIN_HEADERS_MOC}
    ${PLUGIN_FORMS_HEADERS}
    ${PLUGIN_RESOURCES_RCC}
    )
  SET_TARGET_PROPERTIES(${PLUGIN_NAME} PROPERTIES PREFIX "")

  TARGET_LINK_LIBRARIES(${PLUGIN_NAME} ${QT_LIBRARIES} ${PLUGIN_LINK_DEPENDENCIES})
  FOREACH(DEP ${PLUGIN_PKG_CONFIG_DEPENDENCIES})
    PKG_CONFIG_USE_DEPENDENCY(${PLUGIN_NAME} ${DEP})
  ENDFOREACH()

  # Uncomment to install
  INSTALL(TARGETS ${PLUGIN_NAME} DESTINATION lib/gepetto-gui-plugins)
ENDMACRO()
