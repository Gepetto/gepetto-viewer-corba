#
# Copyright (c) 2015-2016 CNRS
# Authors: Joseph Mirabel
#
#
# This file is part of gepetto-gui
# gepetto-gui is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public
# License as published by the Free Software Foundation, either version
# 3 of the License, or (at your option) any later version.
#
# gepetto-gui is distributed in the hope that it will be
# useful, but WITHOUT ANY WARRANTY; without even the implied warranty
# of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
# General Lesser Public License for more details.  You should have
# received a copy of the GNU Lesser General Public License along with
# gepetto-gui  If not, see
# <http://www.gnu.org/licenses/>.

# Configure the project
SET (${PROJECT_NAME}_SOURCES
  ${CMAKE_BINARY_DIR}/src/gui/main.cc
  settings.cc
  windows-manager.cc
  safeapplication.cc
  mainwindow.cc
  osgwidget.cc
  pick-handler.cc
  tree-item.cc
  omniorbthread.cc
  bodytreewidget.cc
  ledindicator.cc
  dialog/dialogloadrobot.cc
  dialog/dialogloadenvironment.cc
  dialog/pluginmanagerdialog.cc
  shortcut-factory.cc
  selection-handler.cc
  selection-event.cc
  action-search-bar.cc
  node-action.cc
  )

IF(GEPETTO_GUI_HAS_PYTHONQT)
  INCLUDE_DIRECTORIES(SYSTEM "${PYTHON_INCLUDE_DIR}" "${PYTHONQT_INCLUDE_DIR}")
  SET (${PROJECT_NAME}_SOURCES
    ${${PROJECT_NAME}_SOURCES}
    pythonwidget.cc)
  SET (${PROJECT_NAME}_MOC
    ${${PROJECT_NAME}_HEADERS_MOC}
    python-decorator.hh
    )
ELSE (GEPETTO_GUI_HAS_PYTHONQT)
  MESSAGE(STATUS "Skipping PythonQt settings")
  SET (${PROJECT_NAME}_MOC
    ${${PROJECT_NAME}_HEADERS_MOC}
    )
ENDIF(GEPETTO_GUI_HAS_PYTHONQT)

SET (${PROJECT_NAME}_HEADERS
  ${${PROJECT_NAME}_HEADERS_MOC}
  ${${PROJECT_NAME}_HEADERS_NO_MOC}
  )

# Compile meta-objects and executable
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

ADD_EXECUTABLE(gepetto-gui ${${PROJECT_NAME}_SOURCES}
  ${${PROJECT_NAME}_HEADERS_MOCED}
  ${${PROJECT_NAME}_HEADERS_NO_MOC}
  ${${PROJECT_NAME}_FORMS_HEADERS}
  ${${PROJECT_NAME}_RESOURCES_RCC}
  )

TARGET_LINK_LIBRARIES(gepetto-gui ${PROJECT_NAME} ${QT_LIBRARIES})
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui gepetto-viewer)
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openscenegraph)
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui openthreads)
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui ${OSGQT_LIB_NAME})
PKG_CONFIG_USE_DEPENDENCY(gepetto-gui omniORB4)
IF (GEPETTO_GUI_HAS_PYTHONQT)
  TARGET_LINK_LIBRARIES(gepetto-gui ${PYTHONQT_LIBRARIES})
ENDIF (GEPETTO_GUI_HAS_PYTHONQT)
TARGET_LINK_LIBRARIES(gepetto-gui ${Boost_LIBRARIES})

INSTALL(TARGETS gepetto-gui DESTINATION bin)
