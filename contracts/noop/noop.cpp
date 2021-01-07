/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */

#include <dfciolib/dfcio.hpp>

namespace dfcio {

   class noop: public contract {
      public:
         noop( account_name self ): contract( self ) { }
         void anyaction( account_name from,
                         const std::string& /*type*/,
                         const std::string& /*data*/ )
         {
            require_auth( from );
         }
   };

   DFCIO_ABI( noop, ( anyaction ) )

} /// dfcio
