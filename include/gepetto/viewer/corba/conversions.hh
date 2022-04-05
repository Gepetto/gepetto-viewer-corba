//  Created by Joseph Mirabel in December 2020.
//  Copyright (c) 2020 LAAS-CNRS. All rights reserved.
//

#ifndef GEPETTO_VIEWER_CORBA_CONVERSIONS_HH
#define GEPETTO_VIEWER_CORBA_CONVERSIONS_HH

#include <assert.h>
#include <omniORB4/CORBA.h>

#include <gepetto/viewer/corba/graphical-interface.hh>

namespace gepetto {
namespace viewer {
namespace corba {
using namespace ::gepetto::corbaserver;

#if __cplusplus >= 201103L

template <typename Scalar>
PositionSeq positionSeq(
    std::initializer_list<std::initializer_list<Scalar> > l) {
  Position* posSeq = PositionSeq::allocbuf((CORBA::ULong)l.size());

  PositionSeq seq((CORBA::ULong)l.size(), (CORBA::ULong)l.size(), posSeq);
  CORBA::ULong i = 0;
  for (const std::initializer_list<Scalar>& el : l) {
    assert(el.size() == 3);
    const Scalar* f = el.begin();
    posSeq[i][0] = static_cast<float>(*(f++));
    posSeq[i][1] = static_cast<float>(*(f++));
    posSeq[i][2] = static_cast<float>(*(f++));
    ++i;
  }
  return seq;
}

#endif  // __cplusplus >= 201103L

}  // end of namespace corba
}  // end of namespace viewer
}  // end of namespace gepetto

#endif  // GEPETTO_VIEWER_CORBA_CONVERSIONS_HH
