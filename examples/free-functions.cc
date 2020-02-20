//
//  free-functions.cc
//  Example usage of gepetto-viewer C++ CORBA client interface.
//
//  Created by Joseph Mirabel in 2020.
//  Copyright (c) 2020 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/corba/client.hh>
#include <gepetto/viewer/corba/conversions.hh>
#include <gepetto/viewer/corba/api.hh>

namespace gui = gepetto::viewer::corba;

int main(int, const char **)
{
    gui::connect ("windowName", true);

    if (gui::connected()) gui::gui()->createWindow("window1");
    // Draw a single point
    if (gui::connected())
      gui::gui()->addSphere ("window1/point", 0.005f, gui::white);

#if __cplusplus >= 201103L
    gui::applyConfiguration("window1/point",
        gui::Transform { 0.f, 0.f, 0.f, 0.f, 0.f, 0.f, 1.f, });
    gui::refresh();

    if (gui::connected()) gui::gui()->addCurve("window1/points",
        gepetto::viewer::corba::positionSeq ({ {0., 1., 0.}, {0., 1., 2.}, }),
        gui::green);
#endif

    gui::refresh();

    return 0;
}
