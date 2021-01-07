/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */

#include <dfciolib/dfcio.hpp>

namespace asserter {
   struct assertdef {
      int8_t      condition;
      std::string message;

      DFCLIB_SERIALIZE( assertdef, (condition)(message) )
   };
}
