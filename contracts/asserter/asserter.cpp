/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */

#include <asserter/asserter.hpp> /// defines assert_def struct (abi)

using namespace asserter;

static int global_variable = 45;

extern "C" {
    /// The apply method implements the dispatch of events to this contract
   void apply( uint64_t /* receiver */, uint64_t code, uint64_t action ) {
       require_auth(code);
       if( code == N(asserter) ) {
          if( action == N(procassert) ) {
             assertdef def = dfcio::unpack_action_data<assertdef>();

             // maybe assert?
             dfcio_assert((uint32_t)def.condition, def.message.c_str());
          } else if( action == N(provereset) ) {
             dfcio_assert(global_variable == 45, "Global Variable Initialized poorly");
             global_variable = 100;
          }
       }
    }
}
