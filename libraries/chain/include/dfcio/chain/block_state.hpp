/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */
#pragma once

#include <dfcio/chain/block_header_state.hpp>
#include <dfcio/chain/block.hpp>
#include <dfcio/chain/transaction_metadata.hpp>
#include <dfcio/chain/action_receipt.hpp>

namespace dfcio { namespace chain {

   struct block_state : public block_header_state {
      explicit block_state( const block_header_state& cur ):block_header_state(cur){}
      block_state( const block_header_state& prev, signed_block_ptr b, bool skip_validate_signee );
      block_state( const block_header_state& prev, block_timestamp_type when );
      block_state() = default;

      /// weak_ptr prev_block_state....
      signed_block_ptr                                    block;
      bool                                                validated = false;
      bool                                                in_current_chain = false;

      /// this data is redundant with the data stored in block, but facilitates
      /// recapturing transactions when we pop a block
      vector<transaction_metadata_ptr>                    trxs;
   };

   using block_state_ptr = std::shared_ptr<block_state>;

} } /// namespace dfcio::chain

FC_REFLECT_DERIVED( dfcio::chain::block_state, (dfcio::chain::block_header_state), (block)(validated)(in_current_chain) )
