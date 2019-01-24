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

    if (argc != 2) {
      std::cout << "Usage " << argv[0] << " <urdf-filename>"
        << std::endl;
      return 1;
    }

    Client client (0, NULL);
    client.connect ();

    client.gui()->createWindow("urdf-display");
    // The second argument is deprecated and ignored. File link starting with
    // "package://" are resolved using ROS_PACKAGE_PATH environment variable.
    client.gui()->addURDF("urdf-display/urdf", argv[1], "");

    client.gui()->refresh();

    return 0;
}
