// Copyright (c) 2016, Joseph Mirabel
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of hpp-gui.
// hpp-gui is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// hpp-gui is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// hpp-gui. If not, see <http://www.gnu.org/licenses/>.

#include <gepetto/gui/color-map.hh>
#include <iostream>

// #define STRINGIFY(x) #x
// #define TOSTRING(x) STRINGIFY(x)

#define COUT(msg) \
  do { \
    std::cout << msg << std::endl; \
  } while (0)

#define CHECK(test,msg) \
  do { \
    if (!(test)) { std::cerr << #test << ": " << msg << std::endl; errorCount++; } \
  } while (0)

using namespace gepetto::gui;

int errorCount = 0;

template <std::size_t VC>
std::size_t expected(int v[VC]) {
  std::size_t ret = 0;
  for (std::size_t i = 0; i < VC; ++i) {
    ret += v[i] * (1 << (VC - 1 - i));
  }
  return ret;
}

int main (int, char**) {
  ColorMap c (3); // log2up -> 2
  // COUT(c.log2up_);
  CHECK(c.remap (0) ==  0, "Value is " << c.remap (0));
  CHECK(c.remap (1) ==  2, "Value is " << c.remap (1));
  CHECK(c.remap (2) ==  1, "Value is " << c.remap (2));

  c = ColorMap (4); // log2up -> 3
  // COUT(c.log2up_);
  CHECK(c.remap (0) == 0, "Value is " << c.remap (0));
  CHECK(c.remap (1) == 4, "Value is " << c.remap (1));
  CHECK(c.remap (2) == 2, "Value is " << c.remap (2));
  CHECK(c.remap (3) == 6, "Value is " << c.remap (3));

  c = ColorMap (1024); // log2up -> 11
  // COUT(c.log2up_);
  int exp1[11] = {0,0,0,0,0,0,0,0,0,0};
  CHECK(c.remap (0) == expected<10>(exp1), "Value is " << c.remap (0));
  int exp2[11] = {1,0,0,0,0,0,0,0,0,0};
  CHECK(c.remap (1) == expected<11>(exp2), "Value is " << c.remap (1));
  int exp3[11] = {1,1,0,0,0,0,0,0,0,0};
  CHECK(c.remap (3) == expected<11>(exp3), "Value is " << c.remap (3));
  int exp4[11] = {0,0,0,1,0,0,0,0,0,0};
  CHECK(c.remap (8) == expected<11>(exp4), "Value is " << c.remap (8));

  if (errorCount == 0) std::cout << "No error detected" << std::endl;
  else std::cout << errorCount << " error(s) detected" << std::endl;
  return errorCount;
}
