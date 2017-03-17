/**
 * Arduheater - Heat controller for astronomy usage
 * Copyright (C) 2016-2017 João Brázio [joao@brazio.org]
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef __PRINT_H__
#define __PRINT_H__

#include "arduheater.h"

namespace serial {
  namespace print {
    inline void string(const char* s) {
      while (*s)
        serial::write(*s++);
    }

    inline void PGM(const char* s) {
      char c;
      while ((c = pgm_read_byte(s))) {
        serial::write(c);
        ++s;
      }
    }
  }
};

namespace serial {
  namespace print {
    namespace chr {
      inline void eol() {
        serial::print::PGM(string_eol);
      }

      inline void space() {
        serial::print::PGM(string_space);
      }

      inline void comma() {
        serial::print::PGM(string_comma);
      }

      inline void colon() {
        serial::print::PGM(string_colon);
      }

      inline void minus() {
        serial::print::PGM(string_minus);
      }

      inline void zero() {
        serial::print::PGM(string_zero);
      }
    }
  }
};

namespace serial {
  namespace println {
    inline void string(const char* s) {
      serial::print::string(s);
      serial::print::chr::eol();
    }

    inline void PGM(const char* s) {
      serial::print::PGM(s);
      serial::print::chr::eol();
    }
  }
};

#endif
