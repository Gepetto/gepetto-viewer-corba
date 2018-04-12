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

#ifndef GEPETTO_GUI_META_HH
#define GEPETTO_GUI_META_HH

#include <QString>

namespace gepetto {
  namespace gui {
    template <typename T> struct convertTo {
      static inline       T& from (      T& t) { return t; }
      static inline const T& from (const T& t) { return t; }
    };
    template <> struct convertTo<std::string> {
      static inline std::string from (const QString& in) { return in.toStdString(); };
    };

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
