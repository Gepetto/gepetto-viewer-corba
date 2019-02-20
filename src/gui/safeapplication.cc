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

#include "safeapplication.hh"

#include <QDebug>
#include <omniORB4/CORBA.h>
#include <gepetto/gui/mainwindow.hh>
#include <gepetto/gui/plugin-interface.hh>

SafeApplication::SafeApplication(int& argc, char** argv) :
  QApplication(argc, argv)
{
}

bool SafeApplication::notify(QObject *receiver, QEvent *e)
{
  try {
    return QApplication::notify(receiver, e);
  } catch (const std::exception& e) {
    qDebug () << e.what();
  } catch (const CORBA::TRANSIENT& e) {
    emit log (QString ("CORBA Exception %1 - %2.<br/><b>You may need to reset the connections (see Tools menu).<b/>").arg(e._name()).arg(e._rep_id()));
  } catch (const CORBA::COMM_FAILURE& e) {
    emit log (QString ("CORBA Exception %1 - %2.<br/><b>You may need to reset the connections (see Tools menu).<b/>").arg(e._name()).arg(e._rep_id()));
  } catch (const CORBA::MARSHAL& e) {
    emit log (QString ("CORBA Exception %1 - %2.<br/><b>A CORBA client or server is not up to date.</b>").arg(e._name()).arg(e._rep_id()));
  } catch (const CORBA::Exception& e) {
    emit log (QString ("Unhandled CORBA Exception %1 - %2").arg(e._name()).arg(e._rep_id()));
  } catch (...) {
    qDebug() << "Unknown exception";
    qDebug() << "Catch it in SafeApplication::notify";
  }
  return false;
}
