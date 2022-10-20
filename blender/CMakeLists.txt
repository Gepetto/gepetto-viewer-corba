# Copyright (c) 2015 CNRS Author: Joseph Mirabel
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

set(BLENDER_EXE
    ""
    CACHE FILEPATH "Path to the blender executable")

if(EXISTS ${BLENDER_EXE})
  configure_file(install_addon.py.in
                 ${CMAKE_CURRENT_BINARY_DIR}/install_addon.py)
  # ADD_CUSTOM_COMMAND (OUTPUT nothing COMMAND echo "${BLENDER_EXE}" COMMENT
  # "Installing the blender addon") COMMAND ${BLENDER_EXE} --background
  # --enable-autoexec --python ${CMAKE_CURRENT_SOURCE_DIR}/install_addon.py
  add_custom_target(
    blender
    PYTHONPATH=/usr/lib/python3/dist-packages/ ${BLENDER_EXE} --background
    --enable-autoexec --python ${CMAKE_CURRENT_BINARY_DIR}/install_addon.py
    COMMENT "Installing the blender addon")
endif(EXISTS ${BLENDER_EXE})

install(PROGRAMS urdf_to_blender.py DESTINATION bin)
