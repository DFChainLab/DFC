#include <dfciolib/dfcio.hpp>
#include <dfciolib/print.hpp>
using namespace dfcio;

class payloadless : public dfcio::contract {
  public:
      using contract::contract;

      void doit() {
         print( "Im a payloadless action" );
      }
};

DFCIO_ABI( payloadless, (doit) )
