/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */
#pragma once

#include <dfcio/chain/types.hpp>
#include <dfcio/chain/contract_types.hpp>

namespace dfcio { namespace chain {

   class apply_context;

   /**
    * @defgroup native_action_handlers Native Action Handlers
    */
   ///@{
   void apply_dfcio_newaccount(apply_context&);
   void apply_dfcio_updateauth(apply_context&);
   void apply_dfcio_deleteauth(apply_context&);
   void apply_dfcio_linkauth(apply_context&);
   void apply_dfcio_unlinkauth(apply_context&);

   /*
   void apply_dfcio_postrecovery(apply_context&);
   void apply_dfcio_passrecovery(apply_context&);
   void apply_dfcio_vetorecovery(apply_context&);
   */

   void apply_dfcio_setcode(apply_context&);
   void apply_dfcio_setabi(apply_context&);

   void apply_dfcio_canceldelay(apply_context&);
   ///@}  end action handlers

} } /// namespace dfcio::chain
