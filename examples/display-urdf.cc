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
    if (argc != 2) {
      std::cout << "Usage " << argv[0] << " <urdf-filename>"
        << std::endl;
      return 1;
    }

    gepetto::viewer::corba::Client client (0, NULL);
    client.connect ();

    client.gui()->createWindow("urdf-display");
    client.gui()->addURDF("urdf-display/urdf", argv[1]);

    client.gui()->refresh();

    return 0;
}
