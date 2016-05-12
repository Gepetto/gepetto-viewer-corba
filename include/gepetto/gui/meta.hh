#ifndef GEPETTO_GUI_META_HH
#define GEPETTO_GUI_META_HH

#include <QString>

namespace gepetto {
  namespace gui {
    template <typename T> struct Traits;

    template <> struct Traits <CORBA::String_var> {
      typedef CORBA::String_var type;
      static inline type from (const QString& s) { return (const char*)s.toLocal8Bit().data(); }
      static inline type from (const std::string& s) { return s.c_str(); }
    };

    template <> struct Traits <QString> {
      typedef CORBA::String_var CORBA_t;
      static inline CORBA_t to_corba (const QString& s) { return (const char*)s.toLocal8Bit().data(); }
    };


    template <typename In, typename Out, std::size_t Size>
    inline void convertSequence (const In* in, Out (&out)[Size]) {
      for (size_t i = 0; i < Size; ++i) out[i] = (Out)in[i];
    }
    template <typename In, typename Out>
    inline void convertSequence (const In* in, Out* out, const std::size_t& s) {
      for (size_t i = 0; i < s; ++i) out[i] = (Out)in[i];
    }
  }
}

#endif // GEPETTO_GUI_META_HH
