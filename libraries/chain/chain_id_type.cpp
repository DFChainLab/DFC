/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */

#include <dfcio/chain/chain_id_type.hpp>
#include <dfcio/chain/exceptions.hpp>

namespace dfcio { namespace chain {

   void chain_id_type::reflector_init()const {
      DFC_ASSERT( *reinterpret_cast<const fc::sha256*>(this) != fc::sha256(), chain_id_type_exception, "chain_id_type cannot be zero" );
   }

} }  // namespace dfcio::chain

namespace fc {

   void to_variant(const dfcio::chain::chain_id_type& cid, fc::variant& v) {
      to_variant( static_cast<const fc::sha256&>(cid), v);
   }

   void from_variant(const fc::variant& v, dfcio::chain::chain_id_type& cid) {
      from_variant( v, static_cast<fc::sha256&>(cid) );
   }

} // fc
