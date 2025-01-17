/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */
#pragma once

#include <dfcio/chain/account_object.hpp>
#include <dfcio/chain/controller.hpp>
#include <dfcio/chain/exceptions.hpp>
#include <dfcio/chain/generated_transaction_object.hpp>
#include <dfcio/chain/global_property_object.hpp>
#include <dfcio/chain/permission_link_object.hpp>
#include <dfcio/chain/permission_object.hpp>
#include <dfcio/chain/resource_limits.hpp>
#include <dfcio/chain/resource_limits_private.hpp>
#include <dfcio/chain/trace.hpp>
#include <dfcio/chain_plugin/chain_plugin.hpp>
#include <dfcio/state_history_plugin/state_history_plugin.hpp>

template <typename T>
struct history_serial_wrapper {
   const chainbase::database& db;
   const T&                   obj;
};

template <typename T>
history_serial_wrapper<T> make_history_serial_wrapper(const chainbase::database& db, const T& obj) {
   return {db, obj};
}

template <typename P, typename T>
struct history_context_wrapper {
   const chainbase::database& db;
   const P&                   context;
   const T&                   obj;
};

template <typename P, typename T>
history_context_wrapper<P, T> make_history_context_wrapper(const chainbase::database& db, P& context, const T& obj) {
   return {db, context, obj};
}

namespace fc {

template <typename T>
const T& as_type(const T& x) {
   return x;
}

template <typename ST, typename T>
datastream<ST>& history_serialize_container(datastream<ST>& ds, const chainbase::database& db, const T& v) {
   fc::raw::pack(ds, unsigned_int(v.size()));
   for (auto& x : v)
      ds << make_history_serial_wrapper(db, x);
   return ds;
}

template <typename ST, typename T>
datastream<ST>& history_serialize_container(datastream<ST>& ds, const chainbase::database& db,
                                            const std::vector<std::shared_ptr<T>>& v) {
   fc::raw::pack(ds, unsigned_int(v.size()));
   for (auto& x : v) {
      DFC_ASSERT(!!x, dfcio::chain::plugin_exception, "null inside container");
      ds << make_history_serial_wrapper(db, *x);
   }
   return ds;
}

template <typename ST, typename T>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_big_vector_wrapper<T>& obj) {
   FC_ASSERT(obj.obj.size() <= 1024 * 1024 * 1024);
   fc::raw::pack(ds, unsigned_int((uint32_t)obj.obj.size()));
   for (auto& x : obj.obj)
      fc::raw::pack(ds, x);
   return ds;
}

template <typename ST>
void history_pack_big_bytes(datastream<ST>& ds, const dfcio::chain::bytes& v) {
   FC_ASSERT(v.size() <= 1024 * 1024 * 1024);
   fc::raw::pack(ds, unsigned_int((uint32_t)v.size()));
   if (v.size())
      ds.write(&v.front(), (uint32_t)v.size());
}

template <typename ST>
void history_pack_big_bytes(datastream<ST>& ds, const fc::optional<dfcio::chain::bytes>& v) {
   fc::raw::pack(ds, v.valid());
   if (v)
      history_pack_big_bytes(ds, *v);
}

template <typename ST, typename T>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<std::vector<T>>& obj) {
   return history_serialize_container(ds, obj.db, obj.obj);
}

template <typename ST, typename First, typename Second>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<std::pair<First, Second>>& obj) {
   fc::raw::pack(ds, obj.obj.first);
   fc::raw::pack(ds, obj.obj.second);
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::account_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.name.value));
   fc::raw::pack(ds, as_type<uint8_t>(obj.obj.vm_type));
   fc::raw::pack(ds, as_type<uint8_t>(obj.obj.vm_version));
   fc::raw::pack(ds, as_type<bool>(obj.obj.privileged));
   fc::raw::pack(ds, as_type<fc::time_point>(obj.obj.last_code_update));
   fc::raw::pack(ds, as_type<dfcio::chain::digest_type>(obj.obj.code_version));
   fc::raw::pack(ds, as_type<dfcio::chain::block_timestamp_type>(obj.obj.creation_date));
   fc::raw::pack(ds, as_type<dfcio::chain::shared_string>(obj.obj.code));
   fc::raw::pack(ds, as_type<dfcio::chain::shared_string>(obj.obj.abi));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::table_id_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.code.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.scope.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.table.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.payer.value));
   return ds;
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                                     ds,
           const history_context_wrapper<const dfcio::chain::table_id_object, dfcio::chain::key_value_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.context.code.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.context.scope.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.context.table.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.primary_key));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.payer.value));
   fc::raw::pack(ds, as_type<dfcio::chain::shared_string>(obj.obj.value));
   return ds;
}

template <typename ST, typename T>
void serialize_secondary_index_data(datastream<ST>& ds, const T& obj) {
   fc::raw::pack(ds, obj);
}

template <typename ST>
void serialize_secondary_index_data(datastream<ST>& ds, const float64_t& obj) {
   uint64_t i;
   memcpy(&i, &obj, sizeof(i));
   fc::raw::pack(ds, i);
}

template <typename ST>
void serialize_secondary_index_data(datastream<ST>& ds, const float128_t& obj) {
   __uint128_t i;
   memcpy(&i, &obj, sizeof(i));
   fc::raw::pack(ds, i);
}

template <typename ST>
void serialize_secondary_index_data(datastream<ST>& ds, const dfcio::chain::key256_t& obj) {
   auto rev = [&](__uint128_t x) {
      char* ch = reinterpret_cast<char*>(&x);
      std::reverse(ch, ch + sizeof(x));
      return x;
   };
   fc::raw::pack(ds, rev(obj[0]));
   fc::raw::pack(ds, rev(obj[1]));
}

template <typename ST, typename T>
datastream<ST>& serialize_secondary_index(datastream<ST>& ds, const dfcio::chain::table_id_object& context,
                                          const T& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(context.code.value));
   fc::raw::pack(ds, as_type<uint64_t>(context.scope.value));
   fc::raw::pack(ds, as_type<uint64_t>(context.table.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.primary_key));
   fc::raw::pack(ds, as_type<uint64_t>(obj.payer.value));
   serialize_secondary_index_data(ds, obj.secondary_key);
   return ds;
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                                   ds,
           const history_context_wrapper<const dfcio::chain::table_id_object, dfcio::chain::index64_object>& obj) {
   return serialize_secondary_index(ds, obj.context, obj.obj);
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                                    ds,
           const history_context_wrapper<const dfcio::chain::table_id_object, dfcio::chain::index128_object>& obj) {
   return serialize_secondary_index(ds, obj.context, obj.obj);
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                                    ds,
           const history_context_wrapper<const dfcio::chain::table_id_object, dfcio::chain::index256_object>& obj) {
   return serialize_secondary_index(ds, obj.context, obj.obj);
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                                        ds,
           const history_context_wrapper<const dfcio::chain::table_id_object, dfcio::chain::index_double_object>& obj) {
   return serialize_secondary_index(ds, obj.context, obj.obj);
}

template <typename ST>
datastream<ST>& operator<<(
    datastream<ST>&                                                                                             ds,
    const history_context_wrapper<const dfcio::chain::table_id_object, dfcio::chain::index_long_double_object>& obj) {
   return serialize_secondary_index(ds, obj.context, obj.obj);
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::producer_key>& obj) {
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.producer_name.value));
   fc::raw::pack(ds, as_type<dfcio::chain::public_key_type>(obj.obj.block_signing_key));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                            ds,
                           const history_serial_wrapper<dfcio::chain::shared_producer_schedule_type>& obj) {
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.version));
   history_serialize_container(ds, obj.db,
                               as_type<dfcio::chain::shared_vector<dfcio::chain::producer_key>>(obj.obj.producers));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::chain_config>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.max_block_net_usage));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.target_block_net_usage_pct));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_transaction_net_usage));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.base_per_transaction_net_usage));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.net_usage_leeway));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.context_free_discount_net_usage_num));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.context_free_discount_net_usage_den));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_block_cpu_usage));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.target_block_cpu_usage_pct));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_transaction_cpu_usage));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.min_transaction_cpu_usage));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_transaction_lifetime));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.deferred_trx_expiration_window));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_transaction_delay));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_inline_action_size));
   fc::raw::pack(ds, as_type<uint16_t>(obj.obj.max_inline_action_depth));
   fc::raw::pack(ds, as_type<uint16_t>(obj.obj.max_authority_depth));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                     ds,
                           const history_serial_wrapper<dfcio::chain::global_property_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<optional<dfcio::chain::block_num_type>>(obj.obj.proposed_schedule_block_num));
   fc::raw::pack(ds, make_history_serial_wrapper(
                         obj.db, as_type<dfcio::chain::shared_producer_schedule_type>(obj.obj.proposed_schedule)));
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::chain_config>(obj.obj.configuration)));

   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                           ds,
                           const history_serial_wrapper<dfcio::chain::generated_transaction_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.sender.value));
   fc::raw::pack(ds, as_type<__uint128_t>(obj.obj.sender_id));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.payer.value));
   fc::raw::pack(ds, as_type<dfcio::chain::transaction_id_type>(obj.obj.trx_id));
   fc::raw::pack(ds, as_type<dfcio::chain::shared_string>(obj.obj.packed_trx));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::key_weight>& obj) {
   fc::raw::pack(ds, as_type<dfcio::chain::public_key_type>(obj.obj.key));
   fc::raw::pack(ds, as_type<uint16_t>(obj.obj.weight));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::permission_level>& obj) {
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.actor.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.permission.value));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                      ds,
                           const history_serial_wrapper<dfcio::chain::permission_level_weight>& obj) {
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::permission_level>(obj.obj.permission)));
   fc::raw::pack(ds, as_type<uint16_t>(obj.obj.weight));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::wait_weight>& obj) {
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.wait_sec));
   fc::raw::pack(ds, as_type<uint16_t>(obj.obj.weight));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::shared_authority>& obj) {
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.threshold));
   history_serialize_container(ds, obj.db, obj.obj.keys);
   history_serialize_container(ds, obj.db, obj.obj.accounts);
   history_serialize_container(ds, obj.db, obj.obj.waits);
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::permission_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.owner.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.name.value));
   if (obj.obj.parent._id) {
      auto&       index  = obj.db.get_index<dfcio::chain::permission_index>();
      const auto* parent = index.find(obj.obj.parent);
      if (!parent) {
         auto& undo = index.stack().back();
         auto  it   = undo.removed_values.find(obj.obj.parent);
         DFC_ASSERT(it != undo.removed_values.end(), dfcio::chain::plugin_exception,
                    "can not find parent of permission_object");
         parent = &it->second;
      }
      fc::raw::pack(ds, as_type<uint64_t>(parent->name.value));
   } else {
      fc::raw::pack(ds, as_type<uint64_t>(0));
   }
   fc::raw::pack(ds, as_type<fc::time_point>(obj.obj.last_updated));
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::shared_authority>(obj.obj.auth)));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                     ds,
                           const history_serial_wrapper<dfcio::chain::permission_link_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.account.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.code.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.message_type.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.required_permission.value));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                                      ds,
                           const history_serial_wrapper<dfcio::chain::resource_limits::resource_limits_object>& obj) {
   DFC_ASSERT(!obj.obj.pending, dfcio::chain::plugin_exception,
              "accepted_block sent while resource_limits_object in pending state");
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.owner.value));
   fc::raw::pack(ds, as_type<int64_t>(obj.obj.net_weight));
   fc::raw::pack(ds, as_type<int64_t>(obj.obj.cpu_weight));
   fc::raw::pack(ds, as_type<int64_t>(obj.obj.ram_bytes));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                                 ds,
                           const history_serial_wrapper<dfcio::chain::resource_limits::usage_accumulator>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.last_ordinal));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.value_ex));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.consumed));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                                     ds,
                           const history_serial_wrapper<dfcio::chain::resource_limits::resource_usage_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.owner.value));
   fc::raw::pack(ds, make_history_serial_wrapper(
                         obj.db, as_type<dfcio::chain::resource_limits::usage_accumulator>(obj.obj.net_usage)));
   fc::raw::pack(ds, make_history_serial_wrapper(
                         obj.db, as_type<dfcio::chain::resource_limits::usage_accumulator>(obj.obj.cpu_usage)));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.ram_usage));
   return ds;
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                            ds,
           const history_serial_wrapper<dfcio::chain::resource_limits::resource_limits_state_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::resource_limits::usage_accumulator>(
                                                             obj.obj.average_block_net_usage)));
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::resource_limits::usage_accumulator>(
                                                             obj.obj.average_block_cpu_usage)));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.total_net_weight));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.total_cpu_weight));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.total_ram_bytes));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.virtual_net_limit));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.virtual_cpu_limit));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                     ds,
                           const history_serial_wrapper<dfcio::chain::resource_limits::ratio>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.numerator));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.denominator));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                                        ds,
                           const history_serial_wrapper<dfcio::chain::resource_limits::elastic_limit_parameters>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.target));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.max));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.periods));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.max_multiplier));
   fc::raw::pack(
       ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::resource_limits::ratio>(obj.obj.contract_rate)));
   fc::raw::pack(
       ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::resource_limits::ratio>(obj.obj.expand_rate)));
   return ds;
}

template <typename ST>
datastream<ST>&
operator<<(datastream<ST>&                                                                             ds,
           const history_serial_wrapper<dfcio::chain::resource_limits::resource_limits_config_object>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(
       ds, make_history_serial_wrapper(
               obj.db, as_type<dfcio::chain::resource_limits::elastic_limit_parameters>(obj.obj.cpu_limit_parameters)));
   fc::raw::pack(
       ds, make_history_serial_wrapper(
               obj.db, as_type<dfcio::chain::resource_limits::elastic_limit_parameters>(obj.obj.net_limit_parameters)));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.account_cpu_usage_average_window));
   fc::raw::pack(ds, as_type<uint32_t>(obj.obj.account_net_usage_average_window));
   return ds;
};

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::action>& obj) {
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.account.value));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.name.value));
   history_serialize_container(ds, obj.db, as_type<std::vector<dfcio::chain::permission_level>>(obj.obj.authorization));
   fc::raw::pack(ds, as_type<dfcio::bytes>(obj.obj.data));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::action_receipt>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.receiver.value));
   fc::raw::pack(ds, as_type<dfcio::chain::digest_type>(obj.obj.act_digest));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.global_sequence));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.recv_sequence));
   history_serialize_container(ds, obj.db, as_type<flat_map<dfcio::name, uint64_t>>(obj.obj.auth_sequence));
   fc::raw::pack(ds, as_type<fc::unsigned_int>(obj.obj.code_sequence));
   fc::raw::pack(ds, as_type<fc::unsigned_int>(obj.obj.abi_sequence));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::account_delta>& obj) {
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.account.value));
   fc::raw::pack(ds, as_type<int64_t>(obj.obj.delta));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::action_trace>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::action_receipt>(obj.obj.receipt)));
   fc::raw::pack(ds, make_history_serial_wrapper(obj.db, as_type<dfcio::chain::action>(obj.obj.act)));
   fc::raw::pack(ds, as_type<bool>(obj.obj.context_free));
   fc::raw::pack(ds, as_type<int64_t>(obj.obj.elapsed.count()));
   fc::raw::pack(ds, as_type<std::string>(obj.obj.console));
   history_serialize_container(ds, obj.db, as_type<flat_set<dfcio::chain::account_delta>>(obj.obj.account_ram_deltas));

   fc::optional<std::string> e;
   if (obj.obj.except)
      e = obj.obj.except->to_string();
   fc::raw::pack(ds, as_type<fc::optional<std::string>>(e));

   history_serialize_container(ds, obj.db, as_type<std::vector<dfcio::chain::action_trace>>(obj.obj.inline_traces));
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>&                                                          ds,
                           const history_context_wrapper<uint8_t, dfcio::chain::transaction_trace>& obj) {
   fc::raw::pack(ds, fc::unsigned_int(0));
   fc::raw::pack(ds, as_type<dfcio::chain::transaction_id_type>(obj.obj.id));
   if (obj.obj.receipt) {
      if (obj.obj.failed_dtrx_trace &&
          obj.obj.receipt->status.value == dfcio::chain::transaction_receipt_header::soft_fail)
         fc::raw::pack(ds, uint8_t(dfcio::chain::transaction_receipt_header::executed));
      else
         fc::raw::pack(ds, as_type<uint8_t>(obj.obj.receipt->status.value));
      fc::raw::pack(ds, as_type<uint32_t>(obj.obj.receipt->cpu_usage_us));
      fc::raw::pack(ds, as_type<fc::unsigned_int>(obj.obj.receipt->net_usage_words));
   } else {
      fc::raw::pack(ds, uint8_t(obj.context));
      fc::raw::pack(ds, uint32_t(0));
      fc::raw::pack(ds, fc::unsigned_int(0));
   }
   fc::raw::pack(ds, as_type<int64_t>(obj.obj.elapsed.count()));
   fc::raw::pack(ds, as_type<uint64_t>(obj.obj.net_usage));
   fc::raw::pack(ds, as_type<bool>(obj.obj.scheduled));
   history_serialize_container(ds, obj.db, as_type<std::vector<dfcio::chain::action_trace>>(obj.obj.action_traces));

   fc::optional<std::string> e;
   if (obj.obj.except)
      e = obj.obj.except->to_string();
   fc::raw::pack(ds, as_type<fc::optional<std::string>>(e));

   fc::raw::pack(ds, bool(obj.obj.failed_dtrx_trace));
   if (obj.obj.failed_dtrx_trace) {
      uint8_t stat = dfcio::chain::transaction_receipt_header::hard_fail;
      if (obj.obj.receipt && obj.obj.receipt->status.value == dfcio::chain::transaction_receipt_header::soft_fail)
         stat = dfcio::chain::transaction_receipt_header::soft_fail;
      fc::raw::pack(ds, make_history_context_wrapper(obj.db, stat, *obj.obj.failed_dtrx_trace));
   }

   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const history_serial_wrapper<dfcio::chain::transaction_trace>& obj) {
   uint8_t stat = dfcio::chain::transaction_receipt_header::hard_fail;
   ds << make_history_context_wrapper(obj.db, stat, obj.obj);
   return ds;
}

template <typename ST>
datastream<ST>& operator<<(datastream<ST>& ds, const dfcio::get_blocks_result_v0& obj) {
   fc::raw::pack(ds, obj.head);
   fc::raw::pack(ds, obj.last_irreversible);
   fc::raw::pack(ds, obj.this_block);
   fc::raw::pack(ds, obj.prev_block);
   history_pack_big_bytes(ds, obj.block);
   history_pack_big_bytes(ds, obj.traces);
   history_pack_big_bytes(ds, obj.deltas);
   return ds;
}

} // namespace fc
