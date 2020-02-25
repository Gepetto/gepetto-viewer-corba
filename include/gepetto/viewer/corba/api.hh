#ifndef GEPETTO_VIEWER_CORBA_API_HH
#define GEPETTO_VIEWER_CORBA_API_HH

#include <gepetto/viewer/corba/client.hh>

namespace gepetto {
namespace viewer {
namespace corba {

#if __cplusplus >= 201103L
void applyConfiguration(const char* name, corbaserver::Transform&& t)
{
  if (!connected()) return;
  gui()->applyConfiguration(name, t);
}
#endif // __cplusplus > 201103L

void refresh()
{
  if (!connected()) return;
  gui()->refresh();
}

} //end of namespace corba
} //end of namespace viewer
} //end of namespace gepetto

#endif // GEPETTO_VIEWER_CORBA_API_HH
