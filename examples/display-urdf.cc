//
//  display-urdf.cc
//  Test SceneViewer with C++ CORBA client interface.
//
//  Created by Joseph Mirabel in June 2015.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/corba/client.hh>

int main(int argc, const char ** argv)
{
    using namespace graphics;
    using namespace corbaServer;

    if (argc != 3) {
      std::cout << "Usage " << argv[0] << " <urdf-filename> <path-to-meshes>"
        << std::endl;
      return 1;
    }

    Client client (0, NULL);
    client.connect ();

    Client::WindowID windowId = client.gui()->createWindow("urdf-display");
    client.gui()->createScene("scene-urdf");
    client.gui()->addSceneToWindow("scene-urdf",windowId);
    client.gui()->addURDF("scene-urdf/urdf", argv[1], argv[2]);

    client.gui()->refresh();

    return 0;
}
