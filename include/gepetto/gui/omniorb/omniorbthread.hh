#ifndef GEPETTO_GUI_OMNIORBTHREAD_HH
#define GEPETTO_GUI_OMNIORBTHREAD_HH

#include <assert.h>

#include <QThread>
#include <QMutex>
#include <QTimerEvent>
#include <QElapsedTimer>

#include "gepetto/gui/fwd.hh"
#include "gepetto/viewer/corba/fwd.hh"

namespace gepetto {
  namespace gui {
    class WorkItem {
      public:
        WorkItem () : id_ (idGlobal++) {}
        virtual ~WorkItem () {}
        virtual void performWork () = 0;
        int id () const { return id_; }
      protected:
        void id (int i) { id_ = i; }
      private:
        static int idGlobal;
        int id_;
    };

    template <typename Caller, typename ReturnType>
      class WorkItem_0 : public WorkItem {
        public:
          typedef ReturnType (Caller::* Method) ();
          WorkItem_0 (Caller* c, Method m) : caller (c), method (m) {}
          void performWork () { (caller->*method) (); }
        private:
          Caller* caller;
          Method method;
      };
    template <typename Caller, typename ReturnType, typename Arg0>
      class WorkItem_1 : public WorkItem {
        public:
          typedef ReturnType (Caller::* Method) (Arg0);
          WorkItem_1 (Caller* c, Method m, Arg0 a0) : caller (c), method (m),
          arg0 (a0) {}
          void performWork () { (caller->*method) (arg0); }
        private:
          Caller* caller;
          Method method;
          Arg0 arg0;
      };
    template <typename Caller, typename ReturnType, typename Arg0, typename Arg1>
      class WorkItem_2 : public WorkItem {
        public:
          typedef ReturnType (Caller::* Method) (Arg0,Arg1);
          WorkItem_2 (Caller* c, Method m, Arg0 a0, Arg1 a1) : caller (c), method (m),
          arg0 (a0), arg1 (a1) {}
          void performWork () { (caller->*method) (arg0, arg1); }
        private:
          Caller* caller;
          Method method;
          Arg0 arg0;
          Arg1 arg1;
      };
    template <typename Caller, typename ReturnType,
             typename Arg0, typename Arg1, typename Arg2>
               class WorkItem_3 : public WorkItem {
                 public:
                   typedef ReturnType (Caller::* Method) (Arg0,Arg1,Arg2);
                   WorkItem_3 (Caller* c, Method m, Arg0 a0, Arg1 a1, Arg2 a2) : caller (c), method (m),
                   arg0 (a0), arg1 (a1), arg2 (a2) {}
                   void performWork () { (caller->*method) (arg0, arg1, arg2); }
                 private:
                   Caller* caller;
                   Method method;
                   Arg0 arg0;
                   Arg1 arg1;
                   Arg2 arg2;
               };
    template <typename Caller, typename ReturnType,
             typename Arg0, typename Arg1, typename Arg2, typename Arg3>
               class WorkItem_4 : public WorkItem {
                 public:
                   typedef ReturnType (Caller::* Method) (Arg0,Arg1,Arg2,Arg3);
                   WorkItem_4 (Caller* c, Method m, Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3) : caller (c), method (m),
                   arg0 (a0), arg1 (a1), arg2 (a2), arg3 (a3) {}
                   void performWork () { (caller->*method) (arg0, arg1, arg2, arg3); }
                 private:
                   Caller* caller;
                   Method method;
                   Arg0 arg0;
                   Arg1 arg1;
                   Arg2 arg2;
                   Arg3 arg3;
               };
    template <typename Caller, typename ReturnType,
             typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4>
               class WorkItem_5 : public WorkItem {
                 public:
                   typedef ReturnType (Caller::* Method) (Arg0,Arg1,Arg2,Arg3,Arg4);
                   WorkItem_5 (Caller* c, Method m, Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4) : caller (c), method (m),
                   arg0 (a0), arg1 (a1), arg2 (a2), arg3 (a3), arg4 (a4) {}
                   void performWork () { (caller->*method) (arg0, arg1, arg2, arg3, arg4); }
                 private:
                   Caller* caller;
                   Method method;
                   Arg0 arg0;
                   Arg1 arg1;
                   Arg2 arg2;
                   Arg3 arg3;
                   Arg4 arg4;
               };
    template <typename Caller, typename ReturnType,
             typename Arg0, typename Arg1, typename Arg2, typename Arg3, typename Arg4, typename Arg5>
               class WorkItem_6 : public WorkItem {
                 public:
                   typedef ReturnType (Caller::* Method) (Arg0,Arg1,Arg2,Arg3,Arg4,Arg5);
                   WorkItem_6 (Caller* c, Method m, Arg0 a0, Arg1 a1, Arg2 a2, Arg3 a3, Arg4 a4, Arg5 a5) : caller (c), method (m),
                   arg0 (a0), arg1 (a1), arg2 (a2), arg3 (a3), arg4 (a4), arg5 (a5) {}
                   void performWork () { (caller->*method) (arg0, arg1, arg2, arg3, arg4, arg5); }
                 private:
                   Caller* caller;
                   Method method;
                   Arg0 arg0;
                   Arg1 arg1;
                   Arg2 arg2;
                   Arg3 arg3;
                   Arg4 arg4;
                   Arg5 arg5;
               };

    class ServerProcess : public QObject
    {
      Q_OBJECT

      public:
        ServerProcess () ;
        void waitForInitDone ();

signals:
        void done ();

        public slots:
          virtual void init ();
        virtual void processRequest (bool loop) = 0;

      protected:
        QMutex initDone_;
    };

    class ViewerServerProcess : public ServerProcess
    {
      Q_OBJECT

      public:
        ViewerServerProcess (graphics::corbaServer::Server* server);

        ~ViewerServerProcess ();


        public slots:
          void init ();
        void processRequest (bool loop);

      private:
        graphics::corbaServer::Server* server_;
    };

    class CorbaServer : public QObject
    {
      Q_OBJECT

      public:
        CorbaServer(ServerProcess* process);

        ~CorbaServer ();

        void wait ();

        void waitForInitDone ();

        void start ();

signals:
        void process (bool);

        protected slots:
          void timerEvent(QTimerEvent *event);
        void processed ();

      private:
        ServerProcess* control_;
        QThread worker_;
        int timerId_, interval_;
    };

    class BackgroundQueue : public QObject
    {
      Q_OBJECT

      public:
        BackgroundQueue (QObject *parent = 0);

signals:
        void done (int workId);
        void failed (int workId, const QString& text);

        public slots:
          void perform(WorkItem* item);
    };
  } // namespace gui
} // namespace gepetto

#endif // GEPETTO_GUI_OMNIORBTHREAD_HH
