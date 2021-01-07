#pragma once

#include <dfciolib/dfcio.hpp>

namespace dfcio {

   class sudo : public contract {
      public:
         sudo( account_name self ):contract(self){}

         void exec();

   };

} /// namespace dfcio
