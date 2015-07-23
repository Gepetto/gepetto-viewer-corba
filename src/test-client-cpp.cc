//
//  test-client-cpp.cc
//  Test SceneViewer with C++ CORBA client interface.
//
//  Created by Mathieu Geisert in December 2014.
//  Copyright (c) 2014 LAAS-CNRS. All rights reserved.
//

#include <gepetto/viewer/corba/client.hh>

void se3ToCorba(CORBA::Float* corbaPosition, const se3::SE3& se3position)
{
    Eigen::Quaternion<float> q(se3position.rotation());
    corbaPosition[0] = se3position.translation()(0);
    corbaPosition[1] = se3position.translation()(1);
    corbaPosition[2] = se3position.translation()(2);    
    corbaPosition[3] = q.w();
    corbaPosition[4] = q.x();
    corbaPosition[5] = q.y();
    corbaPosition[6] = q.z();        
}

int main(int, const char **)
{
    using namespace graphics;
    using namespace corbaServer;
    using namespace std;

    Client client (0, NULL);
    client.connect ();

    se3::SE3 position1 = se3::SE3::Random();
    se3::SE3 position2 = se3::SE3::Random();
    se3::SE3 position3 = se3::SE3::Random();
    se3::SE3 position4 = se3::SE3::Random();
    se3::SE3 position5 = se3::SE3::Random();
    CORBA::Float pos[7];

    Client::WindowID windowId = client.gui()->createWindow("window1");
    client.gui()->createScene("scene1");
    client.gui()->addSceneToWindow("scene1",windowId);
    client.gui()->addURDF("scene1/hrp2", "/local/mgeisert/devel/src/hrp2/hrp2_14_description/urdf/hrp2_14_capsule.urdf", "/local/mgeisert/devel/src/hrp2/");

    sleep(5);

    //vector<float> tri01 (3);

    float pos1[3]= {1.,0.,0.};
    float pos2[3] = {0.,1.,0.};
    float pos3[3]= {0.,1.,1.};
    float color[4] = {1.,1.,1.,1.};
    client.gui()->addTriangleFace("scene1/triangle", pos1, pos2, pos3, color);

    sleep(15);


    se3ToCorba (pos, position1);
    client.gui()->applyConfiguration("scene1/hrp2/RLEG_LINK0", pos);
    se3ToCorba (pos, position2);
    client.gui()->applyConfiguration("scene1/hrp2/RLEG_LINK1", pos);
    se3ToCorba (pos, position3);
    client.gui()->applyConfiguration("scene1/hrp2/LLEG_LINK1", pos);
    se3ToCorba (pos, position4);
    client.gui()->applyConfiguration("scene1/hrp2/LLEG_LINK2", pos);
    se3ToCorba (pos, position5);
    client.gui()->applyConfiguration("scene1/hrp2/BODY", pos);

    client.gui()->refresh();

    return 0;
}
