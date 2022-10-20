# Copyright 2016, 2020 CNRS-LAAS
#
# Authors: Joseph Mirabel, Guilhem Saurel
#
# This file is part of hpp-gui hpp-gui is free software: you can redistribute it
# and/or modify it under the terms of the GNU Lesser General Public License as
# published by the Free Software Foundation, either version 3 of the License, or
# (at your option) any later version.
#
# hpp-gui is distributed in the hope that it will be useful, but WITHOUT ANY
# WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
# A PARTICULAR PURPOSE.  See the GNU General Lesser Public License for more
# details. You should have received a copy of the GNU Lesser General Public
# License along with hpp-gui  If not, see <http://www.gnu.org/licenses/>.

# ADD_TESTCASE(NAME)
# ------------------------
#
# Define a test named `NAME'.
#
# This macro will create a binary from `NAME.cc', link it against Boost and add
# it to the test suite.
#

if(PROJECT_USE_QT4)
  include(${QT_USE_FILE})
endif(PROJECT_USE_QT4)
add_definitions(${QT_DEFINITIONS})

macro(ADD_TESTCASE NAME)
  add_unit_test(${NAME} ${NAME}.cc)
  target_link_libraries(${NAME} gepetto-viewer::gepetto-viewer)
endmacro(ADD_TESTCASE)

add_testcase(colormap)
# ADD_TESTCASE(osgviewerQt)

add_python_unit_test("py-trivial" "tests/trivial.py" "src")
