// Copyright (c) 2018, Joseph Mirabel
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
// of MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// General Lesser Public License for more details.  You should have
// received a copy of the GNU Lesser General Public License along with
// gepetto-viewer-corba. If not, see <http://www.gnu.org/licenses/>.

#include "qgv.h"
#include <QObject>

class QGVScene;
class QGVNode;
class QGVEdge;

namespace PyQgv {
  class QGVCORE_EXPORT QGVDecorator : public QObject
  {
    Q_OBJECT

    public slots:

      // ------- QGVScene ------------------------------------------- //
      QGVScene* new_QGVScene(const QString &name);
    QGVScene* new_QGVScene(const QString &name, QObject *parent);
    void delete_QGVScene(QGVScene* s);

    void setGraphAttribute(QGVScene* s, const QString &name, const QString &value);
    void setNodeAttribute (QGVScene* s, const QString &name, const QString &value);
    void setEdgeAttribute (QGVScene* s, const QString &name, const QString &value);

    QGVNode* addNode(QGVScene* s, const QString& label);
    QGVEdge* addEdge(QGVScene* s, QGVNode* source, QGVNode* target);
    QGVEdge* addEdge(QGVScene* s, QGVNode* source, QGVNode* target, const QString& label);
    // QGVSubGraph* addSubGraph(const QString& name, bool cluster=true);

    void setRootNode(QGVScene* s, QGVNode *node);

    // void setNodePositionAttribute();

    void loadLayout (QGVScene* s, const QString &text);
    void applyLayout(QGVScene* s, const QString &algorithm);
    void render     (QGVScene* s, const QString &algorithm);
    void render     (QGVScene* s, const QString &algorithm, const QString file);
    void freeLayout (QGVScene* s);
    void clear      (QGVScene* s);
    // ------- QGVScene ------------------------------------------- //

    // ------- QGVNode  ------------------------------------------- //
    void setAttribute (QGVNode* n, const QString &name, const QString &value);
    QString getAttribute (QGVNode* n, const QString &name);
    // ------- QGVNode  ------------------------------------------- //

    // ------- QGVEdge  ------------------------------------------- //
    void setAttribute (QGVEdge* e, const QString &name, const QString &value);
    QString getAttribute (QGVEdge* e, const QString &name);
    // ------- QGVEdge  ------------------------------------------- //
  };

  /// Register QGV to PythonQt
  void registerQGV ();
} // namespace PyQgv