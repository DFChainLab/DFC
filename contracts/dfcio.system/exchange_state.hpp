#pragma once

#include <dfciolib/asset.hpp>

namespace dfciosystem {
   using dfcio::asset;
   using dfcio::symbol_type;

   typedef double real_type;

   /**
    *  Uses Bancor math to create a 50/50 relay between two asset types. The state of the
    *  bancor exchange is entirely contained within this struct. There are no external
    *  side effects associated with using this API.
    */
   struct exchange_state {
      asset    supply;

      struct connector {
         asset balance;
         double weight = .5;

         DFCLIB_SERIALIZE( connector, (balance)(weight) )
      };

      connector base;
      connector quote;

      uint64_t primary_key()const { return supply.symbol; }

      asset convert_to_exchange( connector& c, asset in ); 
      asset convert_from_exchange( connector& c, asset in );
      asset convert( asset from, symbol_type to );

      DFCLIB_SERIALIZE( exchange_state, (supply)(base)(quote) )
   };

   typedef dfcio::multi_index<N(rammarket), exchange_state> rammarket;

} /// namespace dfciosystem
