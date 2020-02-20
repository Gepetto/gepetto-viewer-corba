//
//  basic-scene.cc
//  Test SceneViewer with C++ CORBA client interface.
//
//  Created by Joseph Mirabel in June 2015.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/corba/client.hh>

int main(int, const char **)
{
    gepetto::viewer::corba::Client client (0, NULL);
    client.connect ("corbaloc:iiop:localhost:12321");

    float white[4] = {1.,1.,1.,1.};
    client.gui()->createWindow("window1");
    client.gui()->addSphere ("window1/sphere", 0.5, white);

    client.gui()->refresh();

    return 0;
}
