#include <dfciolib/dfcio.hpp>

using namespace dfcio;

class hello : public dfcio::contract {
  public:
      using contract::contract;

      /// @abi action 
      void hi( account_name user ) {
         print( "Hello, ", name{user} );
      }
};

DFCIO_ABI( hello, (hi) )
