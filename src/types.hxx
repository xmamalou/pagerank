/*
    Parallel Systems Extracurricular Project -- Pagerank implementation in the context of the Parallel
    Systems Course of the "Computer Engineering" Masters Programme of NKUA
    Copyright (C) 2025 Christoforos-Marios Mamaloukas

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/

#ifndef _TYPES_HXX_
#define _TYPES_HXX_

#include <string_view>

#include <stdint.h>

// --- TYPES --- //

namespace utility {
  struct Options {
    void*            command_p{ nullptr };
    std::string_view appname{};
    //
    bool             do_serial{ true };
    uint32_t         jobs{ 1 };
    uint32_t         dims[2]{ 2, 2 };
    double           dump_fac{ 0.5 };
    uint32_t         iterations{ 1 };
  };

  struct Error {
    // --- TYPES --- //
    enum class ErrorCode : int {
      WRONG_COMMAND_ERR = 1,
      WRONG_OPTION_ERR = 2,
      NO_VALUE_ERR = 3,
      BAD_VALUE_ERR = 4,
      BAD_DUMPING_FAC_ERR = 5,
    };
    // --- FIELDS --- //
    std::string_view erroneous{};
    ErrorCode        error{};
  };

  using Command = void (*)(const Options&);
}

#endif /* _TYPES_HXX */
