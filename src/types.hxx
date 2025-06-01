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
