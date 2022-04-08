// Copyright (c) 2015-2018, LAAS-CNRS
// Authors: Joseph Mirabel (joseph.mirabel@laas.fr)
//
// This file is part of gepetto-viewer-corba.
// gepetto-viewer-corba is free software: you can redistribute it
// and/or modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation, either version
// 3 of the License, or (at your option) any later version.
//
// gepetto-viewer-corba is distributed in the hope that it will be
// useful, but WITHOUT ANY WARRANTY; without even the implied warranty
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the GNU
// General Lesser Public License for more details. You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#ifndef GEPETTO_GUI_OMNIORB_URL_HH
#define GEPETTO_GUI_OMNIORB_URL_HH

#include <QString>
#include <gepetto/viewer/corba/deprecated.hh>
#warning "This file will be deleted in the future."

namespace gepetto {
namespace gui {
namespace omniOrb {
static inline QString IIOPurl(const QString& host = "localhost",
                              const QString& port = "2809")
    GEPETTO_VIEWER_CORBA_DEPRECATED;

static inline QString IIOPurl(const QString& host, const QString& port) {
  return QString("corbaloc:iiop:%1:%2").arg(host).arg(port);
}
}  // namespace omniOrb
}  // namespace gui
}  // namespace gepetto

#endif  // GEPETTO_GUI_OMNIORB_URL_HH
