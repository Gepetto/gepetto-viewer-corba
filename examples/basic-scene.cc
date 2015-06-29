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
    using namespace graphics;
    using namespace corbaServer;
    using namespace std;

    Client client (0, NULL);
    client.connect ();

    Client::WindowID windowId = client.gui()->createWindow("window1");
    client.gui()->createScene("scene1");
    client.gui()->addSceneToWindow("scene1",windowId);
    float white[4] = {1.,1.,1.,1.};
    client.gui()->addSphere ("scene1/sphere", 0.5, white);

    client.gui()->refresh();

    return 0;
}
