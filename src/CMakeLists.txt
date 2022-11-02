# Copyright (c) 2014-2016, 2020 LAAS-CNRS Author: Mathieu Geisert, Guilhem
# Saurel
#
# This file is part of SceneViewer-corba. SceneViewer-corba is free software:
# you can redistribute it and/or modify it under the terms of the GNU Lesser
# General Public License as published by the Free Software Foundation, either
# version 3 of the License, or (at your option) any later version.
#
# SceneViewer-corba is distributed in the hope that it will be useful, but
# WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
# FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Lesser Public License
# for more details.  You should have received a copy of the GNU Lesser General
# Public License along with hpp-corbaserver.  If not, see
# <http://www.gnu.org/licenses/>.

set(LIBRARY_NAME ${PROJECT_NAME})

# {{{ Generate C++ and Python files from IDL files.
set(IDL_SOURCES graphical-interface)

omniidl_include_directories(${CMAKE_SOURCE_DIR}/idl)

foreach(IDL ${IDL_SOURCES})
  # C++
  generate_idl_cpp(gepetto/viewer/corba/${IDL}
                   ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer)
  install(FILES ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/${IDL}.hh
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gepetto/viewer/corba)

  # Python
  set(_IDL_PYTHON_ARGUMENTS -C${CMAKE_CURRENT_BINARY_DIR} -K
                            -Wbpackage=gepetto.corbaserver)
  if(PYTHON_VERSION_MAJOR EQUAL 3)
    set(_IDL_PYTHON_ARGUMENTS
        -p${CMAKE_SOURCE_DIR}/cmake/hpp/idl -bomniidl_be_python_with_docstring
        ${_IDL_PYTHON_ARGUMENTS})
  endif(PYTHON_VERSION_MAJOR EQUAL 3)
  generate_idl_python(
    gepetto/viewer/corba/${IDL} ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer
    ARGUMENTS ${_IDL_PYTHON_ARGUMENTS})

  string(REGEX REPLACE "-" "_" IDL_UNDERSCORE "${IDL}")

  install(
    FILES
      ${CMAKE_CURRENT_BINARY_DIR}/gepetto/corbaserver/${IDL_UNDERSCORE}_idl.py
    DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver)
  install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/gepetto/corbaserver/gepetto
                    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/corbaserver/gepetto__POA
          DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver)

  # IDL
  install(FILES ${CMAKE_SOURCE_DIR}/idl/gepetto/viewer/${IDL}.idl
          DESTINATION ${CMAKE_INSTALL_DATAROOTDIR}/idl/gepetto/corbaserver)
endforeach()

add_custom_target(generate_idl_cpp DEPENDS ${ALL_IDL_CPP_STUBS})
add_custom_target(generate_idl_python DEPENDS ${ALL_IDL_PYTHON_STUBS})
# }}}

# {{{ C++ client: generate the library
set(${PROJECT_NAME}_SOURCES
    client.cc
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interface.hh
    ${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba/graphical-interfaceSK.cc)
# }}}

# {{{ C++ application: generate the executable if requested
if(NOT CLIENT_ONLY)
  # {{{ Configure Qt
  set(${PROJECT_NAME}_MOC node-callback.hh)
  if(GEPETTO_VIEWER_QTVERSION VERSION_LESS 5)
    qt4_wrap_cpp(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_MOC})
    # QT4_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
    # QT4_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC
    # ${${PROJECT_NAME}_RESOURCES})
  else(GEPETTO_VIEWER_QTVERSION VERSION_LESS 5)
    qt5_wrap_cpp(${PROJECT_NAME}_HEADERS_MOCED ${${PROJECT_NAME}_MOC})
    # QT5_WRAP_UI(${PROJECT_NAME}_FORMS_HEADERS ${${PROJECT_NAME}_FORMS})
    # QT5_ADD_RESOURCES(${PROJECT_NAME}_RESOURCES_RCC
    # ${${PROJECT_NAME}_RESOURCES})
  endif(GEPETTO_VIEWER_QTVERSION VERSION_LESS 5)

  # {{{ Setup source files
  set(${PROJECT_NAME}_SOURCES
      ${${PROJECT_NAME}_SOURCES} graphical-interface.impl.cpp node-callback.cpp
      server.cc server-private.cc ${${PROJECT_NAME}_HEADERS_MOCED})

  add_definitions(${QT_DEFINITIONS})
  # }}}

  # Removes 5 warnings about anonymous variadic macros that were added from c++
  # 11.
  set_source_files_properties(graphical-interface.impl.cpp
                              PROPERTIES COMPILE_FLAGS "-Wno-variadic-macros")
  # }}}

endif(NOT CLIENT_ONLY)
# }}}

add_library(${LIBRARY_NAME} SHARED ${${PROJECT_NAME}_SOURCES})
target_include_directories(${LIBRARY_NAME} PRIVATE ${CMAKE_BINARY_DIR}/src)
add_dependencies(${LIBRARY_NAME} generate_idl_cpp)
add_dependencies(${LIBRARY_NAME} generate_idl_python)
if(NOT CLIENT_ONLY)
  target_link_libraries(${LIBRARY_NAME} PUBLIC gepetto-viewer::gepetto-viewer)
endif(NOT CLIENT_ONLY)
pkg_config_use_dependency(${LIBRARY_NAME} omniORB4)

install(
  TARGETS ${PROJECT_NAME}
  EXPORT ${TARGETS_EXPORT_NAME}
  DESTINATION lib)

# {{{ Python client: install Python files
make_directory(${CMAKE_CURRENT_BINARY_DIR}/gepetto/viewer/corba)

install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/quaternion.py
        ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/color.py
  DESTINATION ${PYTHON_SITELIB}/gepetto)
install(
  FILES ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/__init__.py
        ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/client.py
        ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/exporttoblender.py
        ${CMAKE_CURRENT_SOURCE_DIR}/gepetto/corbaserver/tools.py
  DESTINATION ${PYTHON_SITELIB}/gepetto/corbaserver)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/../blender/gepettoimport.py
        DESTINATION ${PYTHON_SITELIB}/gepetto/blender)
# }}}

# vim: foldmethod=marker foldlevel=0
