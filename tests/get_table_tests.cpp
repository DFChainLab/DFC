#include <boost/test/unit_test.hpp>
#include <boost/algorithm/string/predicate.hpp>

#include <dfcio/testing/tester.hpp>
#include <dfcio/chain/abi_serializer.hpp>
#include <dfcio/chain/wasm_dfcio_constraints.hpp>
#include <dfcio/chain/resource_limits.hpp>
#include <dfcio/chain/exceptions.hpp>
#include <dfcio/chain/wast_to_wasm.hpp>
#include <dfcio/chain_plugin/chain_plugin.hpp>

#include <asserter/asserter.wast.hpp>
#include <asserter/asserter.abi.hpp>

#include <dfcio.token/dfcio.token.wast.hpp>
#include <dfcio.token/dfcio.token.abi.hpp>

#include <dfcio.system/dfcio.system.wast.hpp>
#include <dfcio.system/dfcio.system.abi.hpp>

#include <fc/io/fstream.hpp>

#include <Runtime/Runtime.h>

#include <fc/variant_object.hpp>
#include <fc/io/json.hpp>

#include <array>
#include <utility>

#ifdef NON_VALIDATING_TEST
#define TESTER tester
#else
#define TESTER validating_tester
#endif

using namespace dfcio;
using namespace dfcio::chain;
using namespace dfcio::testing;
using namespace fc;

BOOST_AUTO_TEST_SUITE(get_table_tests)

BOOST_FIXTURE_TEST_CASE( get_scope_test, TESTER ) try {
   produce_blocks(2);

   create_accounts({ N(dfcio.token), N(dfcio.ram), N(dfcio.ramfee), N(dfcio.stake),
      N(dfcio.bpay), N(dfcio.vpay), N(dfcio.saving), N(dfcio.names) });

   std::vector<account_name> accs{N(inita), N(initb), N(initc), N(initd)};
   create_accounts(accs);
   produce_block();

   set_code( N(dfcio.token), dfcio_token_wast );
   set_abi( N(dfcio.token), dfcio_token_abi );
   produce_blocks(1);

   // create currency
   auto act = mutable_variant_object()
         ("issuer",       "dfcio")
         ("maximum_supply", dfcio::chain::asset::from_string("1000000000.0000 SYS"));
   push_action(N(dfcio.token), N(create), N(dfcio.token), act );

   // issue
   for (account_name a: accs) {
      push_action( N(dfcio.token), N(issue), "dfcio", mutable_variant_object()
                  ("to",      name(a) )
                  ("quantity", dfcio::chain::asset::from_string("999.0000 SYS") )
                  ("memo", "")
                  );
   }
   produce_blocks(1);

   // iterate over scope
   dfcio::chain_apis::read_only plugin(*(this->control), fc::microseconds(INT_MAX));
   dfcio::chain_apis::read_only::get_table_by_scope_params param{N(dfcio.token), N(accounts), "inita", "", 10};
   dfcio::chain_apis::read_only::get_table_by_scope_result result = plugin.read_only::get_table_by_scope(param);

   BOOST_REQUIRE_EQUAL(4, result.rows.size());
   BOOST_REQUIRE_EQUAL("", result.more);
   if (result.rows.size() >= 4) {
      BOOST_REQUIRE_EQUAL(name(N(dfcio.token)), result.rows[0].code);
      BOOST_REQUIRE_EQUAL(name(N(inita)), result.rows[0].scope);
      BOOST_REQUIRE_EQUAL(name(N(accounts)), result.rows[0].table);
      BOOST_REQUIRE_EQUAL(name(N(dfcio)), result.rows[0].payer);
      BOOST_REQUIRE_EQUAL(1, result.rows[0].count);

      BOOST_REQUIRE_EQUAL(name(N(initb)), result.rows[1].scope);
      BOOST_REQUIRE_EQUAL(name(N(initc)), result.rows[2].scope);
      BOOST_REQUIRE_EQUAL(name(N(initd)), result.rows[3].scope);
   }

   param.lower_bound = "initb";
   param.upper_bound = "initc";
   result = plugin.read_only::get_table_by_scope(param);
   BOOST_REQUIRE_EQUAL(2, result.rows.size());
   BOOST_REQUIRE_EQUAL("", result.more);
   if (result.rows.size() >= 2) {
      BOOST_REQUIRE_EQUAL(name(N(initb)), result.rows[0].scope);
      BOOST_REQUIRE_EQUAL(name(N(initc)), result.rows[1].scope);
   }

   param.limit = 1;
   result = plugin.read_only::get_table_by_scope(param);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL("initc", result.more);

   param.table = name(0);
   result = plugin.read_only::get_table_by_scope(param);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL("initc", result.more);

   param.table = N(invalid);
   result = plugin.read_only::get_table_by_scope(param);
   BOOST_REQUIRE_EQUAL(0, result.rows.size());
   BOOST_REQUIRE_EQUAL("", result.more);

} FC_LOG_AND_RETHROW() /// get_scope_test

BOOST_FIXTURE_TEST_CASE( get_table_test, TESTER ) try {
   produce_blocks(2);

   create_accounts({ N(dfcio.token), N(dfcio.ram), N(dfcio.ramfee), N(dfcio.stake),
      N(dfcio.bpay), N(dfcio.vpay), N(dfcio.saving), N(dfcio.names) });

   std::vector<account_name> accs{N(inita), N(initb)};
   create_accounts(accs);
   produce_block();

   set_code( N(dfcio.token), dfcio_token_wast );
   set_abi( N(dfcio.token), dfcio_token_abi );
   produce_blocks(1);

   // create currency
   auto act = mutable_variant_object()
         ("issuer",       "dfcio")
         ("maximum_supply", dfcio::chain::asset::from_string("1000000000.0000 SYS"));
   push_action(N(dfcio.token), N(create), N(dfcio.token), act );

   // issue
   for (account_name a: accs) {
      push_action( N(dfcio.token), N(issue), "dfcio", mutable_variant_object()
                  ("to",      name(a) )
                  ("quantity", dfcio::chain::asset::from_string("10000.0000 SYS") )
                  ("memo", "")
                  );
   }
   produce_blocks(1);

   // create currency 2
   act = mutable_variant_object()
         ("issuer",       "dfcio")
         ("maximum_supply", dfcio::chain::asset::from_string("1000000000.0000 AAA"));
   push_action(N(dfcio.token), N(create), N(dfcio.token), act );
   // issue
   for (account_name a: accs) {
      push_action( N(dfcio.token), N(issue), "dfcio", mutable_variant_object()
                  ("to",      name(a) )
                  ("quantity", dfcio::chain::asset::from_string("9999.0000 AAA") )
                  ("memo", "")
                  );
   }
   produce_blocks(1);

   // create currency 3
   act = mutable_variant_object()
         ("issuer",       "dfcio")
         ("maximum_supply", dfcio::chain::asset::from_string("1000000000.0000 CCC"));
   push_action(N(dfcio.token), N(create), N(dfcio.token), act );
   // issue
   for (account_name a: accs) {
      push_action( N(dfcio.token), N(issue), "dfcio", mutable_variant_object()
                  ("to",      name(a) )
                  ("quantity", dfcio::chain::asset::from_string("7777.0000 CCC") )
                  ("memo", "")
                  );
   }
   produce_blocks(1);

   // create currency 3
   act = mutable_variant_object()
         ("issuer",       "dfcio")
         ("maximum_supply", dfcio::chain::asset::from_string("1000000000.0000 BBB"));
   push_action(N(dfcio.token), N(create), N(dfcio.token), act );
   // issue
   for (account_name a: accs) {
      push_action( N(dfcio.token), N(issue), "dfcio", mutable_variant_object()
                  ("to",      name(a) )
                  ("quantity", dfcio::chain::asset::from_string("8888.0000 BBB") )
                  ("memo", "")
                  );
   }
   produce_blocks(1);

   // get table: normal case
   dfcio::chain_apis::read_only plugin(*(this->control), fc::microseconds(INT_MAX));
   dfcio::chain_apis::read_only::get_table_rows_params p;
   p.code = N(dfcio.token);
   p.scope = "inita";
   p.table = N(accounts);
   p.json = true;
   p.index_position = "primary";
   dfcio::chain_apis::read_only::get_table_rows_result result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(4, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 4) {
      BOOST_REQUIRE_EQUAL("9999.0000 AAA", result.rows[0]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("8888.0000 BBB", result.rows[1]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("7777.0000 CCC", result.rows[2]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("10000.0000 SYS", result.rows[3]["balance"].as_string());
   }

   // get table: reverse ordered
   p.reverse = true;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(4, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 4) {
      BOOST_REQUIRE_EQUAL("9999.0000 AAA", result.rows[3]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("8888.0000 BBB", result.rows[2]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("7777.0000 CCC", result.rows[1]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("10000.0000 SYS", result.rows[0]["balance"].as_string());
   }

   // get table: reverse ordered, with ram payer
   p.reverse = true;
   p.show_payer = true;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(4, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 4) {
      BOOST_REQUIRE_EQUAL("9999.0000 AAA", result.rows[3]["data"]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("8888.0000 BBB", result.rows[2]["data"]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("7777.0000 CCC", result.rows[1]["data"]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("10000.0000 SYS", result.rows[0]["data"]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("dfcio", result.rows[0]["payer"].as_string());
      BOOST_REQUIRE_EQUAL("dfcio", result.rows[1]["payer"].as_string());
      BOOST_REQUIRE_EQUAL("dfcio", result.rows[2]["payer"].as_string());
      BOOST_REQUIRE_EQUAL("dfcio", result.rows[3]["payer"].as_string());
   }
   p.show_payer = false;

   // get table: normal case, with bound
   p.lower_bound = "BBB";
   p.upper_bound = "CCC";
   p.reverse = false;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(2, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 2) {
      BOOST_REQUIRE_EQUAL("8888.0000 BBB", result.rows[0]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("7777.0000 CCC", result.rows[1]["balance"].as_string());
   }

   // get table: reverse case, with bound
   p.lower_bound = "BBB";
   p.upper_bound = "CCC";
   p.reverse = true;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(2, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 2) {
      BOOST_REQUIRE_EQUAL("8888.0000 BBB", result.rows[1]["balance"].as_string());
      BOOST_REQUIRE_EQUAL("7777.0000 CCC", result.rows[0]["balance"].as_string());
   }

   // get table: normal case, with limit
   p.lower_bound = p.upper_bound = "";
   p.limit = 1;
   p.reverse = false;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL(true, result.more);
   if (result.rows.size() >= 1) {
      BOOST_REQUIRE_EQUAL("9999.0000 AAA", result.rows[0]["balance"].as_string());
   }

   // get table: reverse case, with limit
   p.lower_bound = p.upper_bound = "";
   p.limit = 1;
   p.reverse = true;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL(true, result.more);
   if (result.rows.size() >= 1) {
      BOOST_REQUIRE_EQUAL("10000.0000 SYS", result.rows[0]["balance"].as_string());
   }

   // get table: normal case, with bound & limit
   p.lower_bound = "BBB";
   p.upper_bound = "CCC";
   p.limit = 1;
   p.reverse = false;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL(true, result.more);
   if (result.rows.size() >= 1) {
      BOOST_REQUIRE_EQUAL("8888.0000 BBB", result.rows[0]["balance"].as_string());
   }

   // get table: reverse case, with bound & limit
   p.lower_bound = "BBB";
   p.upper_bound = "CCC";
   p.limit = 1;
   p.reverse = true;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL(true, result.more);
   if (result.rows.size() >= 1) {
      BOOST_REQUIRE_EQUAL("7777.0000 CCC", result.rows[0]["balance"].as_string());
   }

} FC_LOG_AND_RETHROW()

BOOST_FIXTURE_TEST_CASE( get_table_by_seckey_test, TESTER ) try {
   produce_blocks(2);

   create_accounts({ N(dfcio.token), N(dfcio.ram), N(dfcio.ramfee), N(dfcio.stake),
      N(dfcio.bpay), N(dfcio.vpay), N(dfcio.saving), N(dfcio.names) });

   std::vector<account_name> accs{N(inita), N(initb), N(initc), N(initd)};
   create_accounts(accs);
   produce_block();

   set_code( N(dfcio.token), dfcio_token_wast );
   set_abi( N(dfcio.token), dfcio_token_abi );
   produce_blocks(1);

   // create currency
   auto act = mutable_variant_object()
         ("issuer",       "dfcio")
         ("maximum_supply", dfcio::chain::asset::from_string("1000000000.0000 SYS"));
   push_action(N(dfcio.token), N(create), N(dfcio.token), act );

   // issue
   for (account_name a: accs) {
      push_action( N(dfcio.token), N(issue), "dfcio", mutable_variant_object()
                  ("to",      name(a) )
                  ("quantity", dfcio::chain::asset::from_string("10000.0000 SYS") )
                  ("memo", "")
                  );
   }
   produce_blocks(1);

   set_code( config::system_account_name, dfcio_system_wast );
   set_abi( config::system_account_name, dfcio_system_abi );

   // bidname
   auto bidname = [this]( const account_name& bidder, const account_name& newname, const asset& bid ) {
      return push_action( N(dfcio), N(bidname), bidder, fc::mutable_variant_object()
                          ("bidder",  bidder)
                          ("newname", newname)
                          ("bid", bid)
                          );
   };

   bidname(N(inita), N(com), dfcio::chain::asset::from_string("10.0000 SYS"));
   bidname(N(initb), N(org), dfcio::chain::asset::from_string("11.0000 SYS"));
   bidname(N(initc), N(io), dfcio::chain::asset::from_string("12.0000 SYS"));
   bidname(N(initd), N(html), dfcio::chain::asset::from_string("14.0000 SYS"));
   produce_blocks(1);

   // get table: normal case
   dfcio::chain_apis::read_only plugin(*(this->control), fc::microseconds(INT_MAX));
   dfcio::chain_apis::read_only::get_table_rows_params p;
   p.code = N(dfcio);
   p.scope = "dfcio";
   p.table = N(namebids);
   p.json = true;
   p.index_position = "secondary"; // ordered by high_bid
   p.key_type = "i64";
   dfcio::chain_apis::read_only::get_table_rows_result result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(4, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 4) {
      BOOST_REQUIRE_EQUAL("html", result.rows[0]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initd", result.rows[0]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("140000", result.rows[0]["high_bid"].as_string());

      BOOST_REQUIRE_EQUAL("io", result.rows[1]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initc", result.rows[1]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("120000", result.rows[1]["high_bid"].as_string());

      BOOST_REQUIRE_EQUAL("org", result.rows[2]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initb", result.rows[2]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("110000", result.rows[2]["high_bid"].as_string());

      BOOST_REQUIRE_EQUAL("com", result.rows[3]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("inita", result.rows[3]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("100000", result.rows[3]["high_bid"].as_string());
   }

   // reverse search, with show ram payer
   p.reverse = true;
   p.show_payer = true;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(4, result.rows.size());
   BOOST_REQUIRE_EQUAL(false, result.more);
   if (result.rows.size() >= 4) {
      BOOST_REQUIRE_EQUAL("html", result.rows[3]["data"]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initd", result.rows[3]["data"]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("140000", result.rows[3]["data"]["high_bid"].as_string());
      BOOST_REQUIRE_EQUAL("initd", result.rows[3]["payer"].as_string());

      BOOST_REQUIRE_EQUAL("io", result.rows[2]["data"]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initc", result.rows[2]["data"]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("120000", result.rows[2]["data"]["high_bid"].as_string());
      BOOST_REQUIRE_EQUAL("initc", result.rows[2]["payer"].as_string());

      BOOST_REQUIRE_EQUAL("org", result.rows[1]["data"]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initb", result.rows[1]["data"]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("110000", result.rows[1]["data"]["high_bid"].as_string());
      BOOST_REQUIRE_EQUAL("initb", result.rows[1]["payer"].as_string());

      BOOST_REQUIRE_EQUAL("com", result.rows[0]["data"]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("inita", result.rows[0]["data"]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("100000", result.rows[0]["data"]["high_bid"].as_string());
      BOOST_REQUIRE_EQUAL("inita", result.rows[0]["payer"].as_string());
   }

   // limit to 1 (get the highest bidname)
   p.reverse = false;
   p.show_payer = false;
   p.limit = 1;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL(true, result.more);
   if (result.rows.size() >= 1) {
      BOOST_REQUIRE_EQUAL("html", result.rows[0]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("initd", result.rows[0]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("140000", result.rows[0]["high_bid"].as_string());
   }

   // limit to 1 reverse, (get the lowest bidname)
   p.reverse = true;
   p.show_payer = false;
   p.limit = 1;
   result = plugin.read_only::get_table_rows(p);
   BOOST_REQUIRE_EQUAL(1, result.rows.size());
   BOOST_REQUIRE_EQUAL(true, result.more);
   if (result.rows.size() >= 1) {
      BOOST_REQUIRE_EQUAL("com", result.rows[0]["newname"].as_string());
      BOOST_REQUIRE_EQUAL("inita", result.rows[0]["high_bidder"].as_string());
      BOOST_REQUIRE_EQUAL("100000", result.rows[0]["high_bid"].as_string());
   }

} FC_LOG_AND_RETHROW()

BOOST_AUTO_TEST_SUITE_END()
