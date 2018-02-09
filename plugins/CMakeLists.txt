#
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

#ADD_SUBDIRECTORY(plugin-skeleton)

IF(PYTHONQT_FOUND)
  ADD_OPTIONAL_DEPENDENCY("QGVCore")

  SET(BUILD_PY_QGV       ${QGVCORE_FOUND}
    CACHE BOOL "Build PyQGV plugin")
ENDIF(PYTHONQT_FOUND)

IF(BUILD_PY_QGV)
  ADD_SUBDIRECTORY(pyqgv)
ENDIF(BUILD_PY_QGV)
