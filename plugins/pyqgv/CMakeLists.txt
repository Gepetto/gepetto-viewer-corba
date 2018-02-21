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

SET(PLUGIN_NAME pyqgv)

SET(${PLUGIN_NAME}_HEADERS_NO_MOC
  )

SET(${PLUGIN_NAME}_HEADERS
  plugin.hh
  decorator.hh
  )

SET(${PLUGIN_NAME}_FORMS
  )

SET(${PLUGIN_NAME}_RESOURCES
  )

IF(USE_QT4)
  QT4_WRAP_CPP(${PLUGIN_NAME}_HEADERS_MOC ${${PLUGIN_NAME}_HEADERS})
  QT4_WRAP_UI(${PLUGIN_NAME}_FORMS_HEADERS ${${PLUGIN_NAME}_FORMS})
  QT4_ADD_RESOURCES(${PLUGIN_NAME}_RESOURCES_RCC ${${PLUGIN_NAME}_RESOURCES})
  INCLUDE(${QT_USE_FILE})
ELSE(USE_QT4)
  QT5_WRAP_CPP(${PLUGIN_NAME}_HEADERS_MOC ${${PLUGIN_NAME}_HEADERS})
  QT5_WRAP_UI(${PLUGIN_NAME}_FORMS_HEADERS ${${PLUGIN_NAME}_FORMS})
  QT5_ADD_RESOURCES(${PLUGIN_NAME}_RESOURCES_RCC ${${PLUGIN_NAME}_RESOURCES})
ENDIF(USE_QT4)

INCLUDE_DIRECTORIES("${PYTHON_INCLUDE_DIR}" "${PYTHONQT_INCLUDE_DIR}")
ADD_DEFINITIONS(${QT_DEFINITIONS})
ADD_DEFINITIONS(-DQT_PLUGIN)
ADD_DEFINITIONS(-DQT_NO_DEBUG)
ADD_DEFINITIONS(-DQT_SHARED)

SET (${PLUGIN_NAME}_SOURCES
  plugin.cc
  decorator.cc
  )

ADD_LIBRARY(${PLUGIN_NAME} SHARED ${${PLUGIN_NAME}_SOURCES}
  ${${PLUGIN_NAME}_HEADERS_NO_MOC}
  ${${PLUGIN_NAME}_HEADERS_MOC}
  ${${PLUGIN_NAME}_FORMS_HEADERS}
  ${${PLUGIN_NAME}_RESOURCES_RCC}
  )

TARGET_LINK_LIBRARIES(${PLUGIN_NAME} ${QT_LIBRARIES} ${PYTHONQT_LIBRARIES})
PKG_CONFIG_USE_DEPENDENCY(${PLUGIN_NAME} "QGVCore")

# Uncomment to install
INSTALL(TARGETS ${PLUGIN_NAME} DESTINATION lib/gepetto-gui-plugins)
