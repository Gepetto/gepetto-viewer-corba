# Copyright (c) 2019 CNRS Author: Joseph Mirabel
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

# {{{ Find Qt tags
set(_use_qt_local_doc FALSE)
set(qt_online_doc "http://doc.qt.io")

if(PROJECT_USE_QT4)
  find_file(
    QT4_TAGFILE doc/html/qt.tags
    HINTS / /usr
    PATH_SUFFIXES share/qt4)
  # Eventually, QT4_TAGFILE can be set manually to
  # /usr/share/qt4/doc/html/qt.tags on ubuntu 16.04, from package qt4-doc-html
  if(QT4_TAGFILE)
    if(_use_qt_local_doc)
      get_filename_component(path_to_doc ${QT4_TAGFILE} DIRECTORY)
      set(QT_TAGFILES
          "\"${QT4_TAGFILE}=${path_to_doc}\""
          PARENT_SCOPE)
    else(_use_qt_local_doc)
      set(QT_TAGFILES
          "\"${QT4_TAGFILE}=${qt_doc}/archives/qt-4.8\""
          PARENT_SCOPE)
    endif(_use_qt_local_doc)
  endif()
else(PROJECT_USE_QT4)
  set(_QT_TAGFILES)
  # Available components qtcore qtgui qtwidgets qtnetwork qtsql qtprintsupport
  # qttestlib qtconcurrent qdoc qtxml
  foreach(component "core" "widgets" "gui" "network")
    string(TOUPPER ${component} _up_comp)
    find_file(
      QT5_${_up_comp}_TAGFILE doc/qt${component}/qt${component}.tags
      HINTS / /usr
      PATH_SUFFIXES share/qt5)
    # Eventually, QT5_${_up_comp}_TAGFILE can be set manually to
    # /usr/share/qt5/doc/qt${component}/qt${component}.tags on ubuntu 16.04,
    # from package qtbase5-doc-html
    if(QT5_${_up_comp}_TAGFILE)
      if(_use_qt_local_doc)
        get_filename_component(path_to_doc ${QT5_${_up_comp}_TAGFILE} DIRECTORY)
        set(_QT_TAGFILES
            "${_QT_TAGFILES} \"${QT5_${_up_comp}_TAGFILE}=${path_to_doc}\"")
      else(_use_qt_local_doc)
        set(_QT_TAGFILES
            "${_QT_TAGFILES} \"${QT5_${_up_comp}_TAGFILE}=${qt_doc}/qt-5\"")
      endif(_use_qt_local_doc)
    endif()
  endforeach()
  set(QT_TAGFILES
      ${_QT_TAGFILES}
      PARENT_SCOPE)
endif(PROJECT_USE_QT4)
# }}}

# vim: foldmethod=marker foldlevel=0
