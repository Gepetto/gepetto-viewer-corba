# Copyright (c) 2015, 2020 LAAS-CNRS Author: Joseph Mirabel, Guilhem Saurel
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

macro(ADD_EXAMPLE NAME SOURCE)
  add_executable(${NAME} ${SOURCE})
  target_link_libraries(${NAME} PRIVATE ${LIBRARY_NAME})
  pkg_config_use_dependency(${NAME} omniORB4)
endmacro()

add_example(basic-scene basic-scene.cc)
add_example(display-urdf display-urdf.cc)
add_example(free-functions free-functions.cc)
