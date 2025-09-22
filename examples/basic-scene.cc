//
//  basic-scene.cc
//  Test SceneViewer with C++ CORBA client interface.
//
//  Created by Joseph Mirabel in June 2015.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/corba/client.hh>
#include <gepetto/viewer/corba/conversions.hh>

int main(int, const char**) {
  gepetto::viewer::corba::Client client(0, NULL);
  client.connect("corbaloc:iiop:localhost:12321");

  float white[4] = {1., 1., 1., 1.};
  client.gui()->createWindow("window1");
  client.gui()->addSphere("window1/sphere", 0.5, white);

#if __cplusplus >= 201103L
  client.gui()->addCurve("window1/points",
                         gepetto::viewer::corba::positionSeq({
                             {0., 1., 0.},
                             {0., 1., 2.},
                         }),
                         white);
#endif

  client.gui()->refresh();

  return 0;
}
