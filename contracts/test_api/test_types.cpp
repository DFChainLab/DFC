/**
 *  @file
 *  @copyright defined in dfc/LICENSE
 */
#include <dfciolib/dfcio.hpp>

#include "test_api.hpp"

void test_types::types_size() {

   dfcio_assert( sizeof(int64_t) == 8, "int64_t size != 8");
   dfcio_assert( sizeof(uint64_t) ==  8, "uint64_t size != 8");
   dfcio_assert( sizeof(uint32_t) ==  4, "uint32_t size != 4");
   dfcio_assert( sizeof(int32_t) ==  4, "int32_t size != 4");
   dfcio_assert( sizeof(uint128_t) == 16, "uint128_t size != 16");
   dfcio_assert( sizeof(int128_t) == 16, "int128_t size != 16");
   dfcio_assert( sizeof(uint8_t) ==  1, "uint8_t size != 1");

   dfcio_assert( sizeof(account_name) ==  8, "account_name size !=  8");
   dfcio_assert( sizeof(table_name) ==  8, "table_name size !=  8");
   dfcio_assert( sizeof(time) ==  4, "time size !=  4");
   dfcio_assert( sizeof(dfcio::key256) == 32, "key256 size != 32" );
}

void test_types::char_to_symbol() {

   dfcio_assert( dfcio::char_to_symbol('1') ==  1, "dfcio::char_to_symbol('1') !=  1");
   dfcio_assert( dfcio::char_to_symbol('2') ==  2, "dfcio::char_to_symbol('2') !=  2");
   dfcio_assert( dfcio::char_to_symbol('3') ==  3, "dfcio::char_to_symbol('3') !=  3");
   dfcio_assert( dfcio::char_to_symbol('4') ==  4, "dfcio::char_to_symbol('4') !=  4");
   dfcio_assert( dfcio::char_to_symbol('5') ==  5, "dfcio::char_to_symbol('5') !=  5");
   dfcio_assert( dfcio::char_to_symbol('a') ==  6, "dfcio::char_to_symbol('a') !=  6");
   dfcio_assert( dfcio::char_to_symbol('b') ==  7, "dfcio::char_to_symbol('b') !=  7");
   dfcio_assert( dfcio::char_to_symbol('c') ==  8, "dfcio::char_to_symbol('c') !=  8");
   dfcio_assert( dfcio::char_to_symbol('d') ==  9, "dfcio::char_to_symbol('d') !=  9");
   dfcio_assert( dfcio::char_to_symbol('e') == 10, "dfcio::char_to_symbol('e') != 10");
   dfcio_assert( dfcio::char_to_symbol('f') == 11, "dfcio::char_to_symbol('f') != 11");
   dfcio_assert( dfcio::char_to_symbol('g') == 12, "dfcio::char_to_symbol('g') != 12");
   dfcio_assert( dfcio::char_to_symbol('h') == 13, "dfcio::char_to_symbol('h') != 13");
   dfcio_assert( dfcio::char_to_symbol('i') == 14, "dfcio::char_to_symbol('i') != 14");
   dfcio_assert( dfcio::char_to_symbol('j') == 15, "dfcio::char_to_symbol('j') != 15");
   dfcio_assert( dfcio::char_to_symbol('k') == 16, "dfcio::char_to_symbol('k') != 16");
   dfcio_assert( dfcio::char_to_symbol('l') == 17, "dfcio::char_to_symbol('l') != 17");
   dfcio_assert( dfcio::char_to_symbol('m') == 18, "dfcio::char_to_symbol('m') != 18");
   dfcio_assert( dfcio::char_to_symbol('n') == 19, "dfcio::char_to_symbol('n') != 19");
   dfcio_assert( dfcio::char_to_symbol('o') == 20, "dfcio::char_to_symbol('o') != 20");
   dfcio_assert( dfcio::char_to_symbol('p') == 21, "dfcio::char_to_symbol('p') != 21");
   dfcio_assert( dfcio::char_to_symbol('q') == 22, "dfcio::char_to_symbol('q') != 22");
   dfcio_assert( dfcio::char_to_symbol('r') == 23, "dfcio::char_to_symbol('r') != 23");
   dfcio_assert( dfcio::char_to_symbol('s') == 24, "dfcio::char_to_symbol('s') != 24");
   dfcio_assert( dfcio::char_to_symbol('t') == 25, "dfcio::char_to_symbol('t') != 25");
   dfcio_assert( dfcio::char_to_symbol('u') == 26, "dfcio::char_to_symbol('u') != 26");
   dfcio_assert( dfcio::char_to_symbol('v') == 27, "dfcio::char_to_symbol('v') != 27");
   dfcio_assert( dfcio::char_to_symbol('w') == 28, "dfcio::char_to_symbol('w') != 28");
   dfcio_assert( dfcio::char_to_symbol('x') == 29, "dfcio::char_to_symbol('x') != 29");
   dfcio_assert( dfcio::char_to_symbol('y') == 30, "dfcio::char_to_symbol('y') != 30");
   dfcio_assert( dfcio::char_to_symbol('z') == 31, "dfcio::char_to_symbol('z') != 31");

   for(unsigned char i = 0; i<255; i++) {
      if((i >= 'a' && i <= 'z') || (i >= '1' || i <= '5')) continue;
      dfcio_assert( dfcio::char_to_symbol((char)i) == 0, "dfcio::char_to_symbol() != 0");
   }
}

void test_types::string_to_name() {

   dfcio_assert( dfcio::string_to_name("a") == N(a) , "dfcio::string_to_name(a)" );
   dfcio_assert( dfcio::string_to_name("ba") == N(ba) , "dfcio::string_to_name(ba)" );
   dfcio_assert( dfcio::string_to_name("cba") == N(cba) , "dfcio::string_to_name(cba)" );
   dfcio_assert( dfcio::string_to_name("dcba") == N(dcba) , "dfcio::string_to_name(dcba)" );
   dfcio_assert( dfcio::string_to_name("edcba") == N(edcba) , "dfcio::string_to_name(edcba)" );
   dfcio_assert( dfcio::string_to_name("fedcba") == N(fedcba) , "dfcio::string_to_name(fedcba)" );
   dfcio_assert( dfcio::string_to_name("gfedcba") == N(gfedcba) , "dfcio::string_to_name(gfedcba)" );
   dfcio_assert( dfcio::string_to_name("hgfedcba") == N(hgfedcba) , "dfcio::string_to_name(hgfedcba)" );
   dfcio_assert( dfcio::string_to_name("ihgfedcba") == N(ihgfedcba) , "dfcio::string_to_name(ihgfedcba)" );
   dfcio_assert( dfcio::string_to_name("jihgfedcba") == N(jihgfedcba) , "dfcio::string_to_name(jihgfedcba)" );
   dfcio_assert( dfcio::string_to_name("kjihgfedcba") == N(kjihgfedcba) , "dfcio::string_to_name(kjihgfedcba)" );
   dfcio_assert( dfcio::string_to_name("lkjihgfedcba") == N(lkjihgfedcba) , "dfcio::string_to_name(lkjihgfedcba)" );
   dfcio_assert( dfcio::string_to_name("mlkjihgfedcba") == N(mlkjihgfedcba) , "dfcio::string_to_name(mlkjihgfedcba)" );
   dfcio_assert( dfcio::string_to_name("mlkjihgfedcba1") == N(mlkjihgfedcba2) , "dfcio::string_to_name(mlkjihgfedcba2)" );
   dfcio_assert( dfcio::string_to_name("mlkjihgfedcba55") == N(mlkjihgfedcba14) , "dfcio::string_to_name(mlkjihgfedcba14)" );

   dfcio_assert( dfcio::string_to_name("azAA34") == N(azBB34) , "dfcio::string_to_name N(azBB34)" );
   dfcio_assert( dfcio::string_to_name("AZaz12Bc34") == N(AZaz12Bc34) , "dfcio::string_to_name AZaz12Bc34" );
   dfcio_assert( dfcio::string_to_name("AAAAAAAAAAAAAAA") == dfcio::string_to_name("BBBBBBBBBBBBBDDDDDFFFGG") , "dfcio::string_to_name BBBBBBBBBBBBBDDDDDFFFGG" );
}

void test_types::name_class() {

   dfcio_assert( dfcio::name{dfcio::string_to_name("azAA34")}.value == N(azAA34), "dfcio::name != N(azAA34)" );
   dfcio_assert( dfcio::name{dfcio::string_to_name("AABBCC")}.value == 0, "dfcio::name != N(0)" );
   dfcio_assert( dfcio::name{dfcio::string_to_name("AA11")}.value == N(AA11), "dfcio::name != N(AA11)" );
   dfcio_assert( dfcio::name{dfcio::string_to_name("11AA")}.value == N(11), "dfcio::name != N(11)" );
   dfcio_assert( dfcio::name{dfcio::string_to_name("22BBCCXXAA")}.value == N(22), "dfcio::name != N(22)" );
   dfcio_assert( dfcio::name{dfcio::string_to_name("AAAbbcccdd")} == dfcio::name{dfcio::string_to_name("AAAbbcccdd")}, "dfcio::name == dfcio::name" );

   uint64_t tmp = dfcio::name{dfcio::string_to_name("11bbcccdd")};
   dfcio_assert(N(11bbcccdd) == tmp, "N(11bbcccdd) == tmp");
}
