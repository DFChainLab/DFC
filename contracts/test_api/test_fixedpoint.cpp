#include <dfciolib/fixedpoint.hpp>
#include <dfciolib/dfcio.hpp>

#include "test_api.hpp"

void test_fixedpoint::create_instances()
{
    {
        // Various ways to create fixed_point128
       dfcio::fixed_point128<18> a(12345667);
       dfcio::fixed_point128<18> b(12345667);
       dfcio::fixed_point128<16> c(12345667);
       dfcio_assert(b == a, "fixed_point128 instances comparison with same number of decimals");
       dfcio_assert(c == a, "fixed_point128 instances with different number of decimals");
    }

    {
        // Various ways to create fixed_point64
       dfcio::fixed_point64<5> a(12345667);
       dfcio::fixed_point64<5> b(12345667);
       dfcio::fixed_point64<5> c(12345667);
       dfcio_assert(b == a, "fixed_point64 instances comparison with same number of decimals");
       dfcio_assert(c == a, "fixed_point64 instances with different number of decimals");
    }

    {
        // Various ways to create fixed_point32
       dfcio::fixed_point32<18> a(12345667);
       dfcio::fixed_point32<18> b(12345667);
       dfcio::fixed_point32<16> c(12345667);
       dfcio_assert(b == a, "fixed_point32 instances comparison with same number of decimals");
       dfcio_assert(c == a, "fixed_point32 instances with different number of decimals");
    }
}

void test_fixedpoint::test_addition()
{
    {
       // Various ways to create fixed_point32
       dfcio::fixed_point32<0> a(100);
       dfcio::fixed_point32<0> b(100);
       dfcio::fixed_point32<0> c = a + b;
       dfcio::fixed_point32<0> d = 200;
       dfcio_assert(c == d, "fixed_point32 instances addition with zero decmimals");
    }
    {
       // Various ways to create fixed_point64
       dfcio::fixed_point64<0> a(100);
       dfcio::fixed_point64<0> b(100);
       dfcio::fixed_point64<0> c = a + b;
       dfcio::fixed_point64<0> d = 200;
       dfcio_assert(c == d, "fixed_point64 instances addition with zero decmimals");
    }
};

void test_fixedpoint::test_subtraction()
{
    {
       // Various ways to create fixed_point64
       dfcio::fixed_point64<0> a(100);
       dfcio::fixed_point64<0> b(100);
       dfcio::fixed_point64<0> c = a - b;
       dfcio::fixed_point64<0> d = 0;
       dfcio_assert(c == d, "fixed_point64 instances subtraction with zero decmimals");

       dfcio::fixed_point64<0> a1(0);
       dfcio::fixed_point64<0> c1 = a1 - b;
       dfcio::fixed_point64<0> d1 = -100;
       dfcio_assert(c1 == d1, "fixed_point64 instances subtraction with zero decmimals");
    }
    {
       // Various ways to create fixed_point32
       dfcio::fixed_point32<0> a(100);
       dfcio::fixed_point32<0> b(100);
       dfcio::fixed_point32<0> c = a - b;
       dfcio::fixed_point32<0> d = 0;
       dfcio_assert(c == d, "fixed_point32 instances subtraction with zero decmimals");

       // Various ways to create fixed_point32
       dfcio::fixed_point32<0> a1(0);
       dfcio::fixed_point32<0> c1 = a1 - b;
       dfcio::fixed_point32<0> d1 = -100;
       dfcio_assert(c1 == d1, "fixed_point32 instances subtraction with zero decmimals");

    }
};

void test_fixedpoint::test_multiplication()
{
    {
       // Various ways to create fixed_point64
       dfcio::fixed_point64<0> a(100);
       dfcio::fixed_point64<0> b(200);
       dfcio::fixed_point128<0> c = a * b;
       dfcio::fixed_point128<0> d(200*100);
       dfcio_assert(c == d, "fixed_point64 instances multiplication result in fixed_point128");
    }

    {
       // Various ways to create fixed_point32
       dfcio::fixed_point32<0> a(100);
       dfcio::fixed_point32<0> b(200);
       dfcio::fixed_point64<0> c = a * b;
       dfcio::fixed_point64<0> d(200*100);
       dfcio_assert(c == d, "fixed_point32 instances multiplication result in fixed_point64");
    }
}

void test_fixedpoint::test_division()
{
    {
        uint64_t lhs = 10000000;
        uint64_t rhs = 333;

        dfcio::fixed_point64<0> a((int64_t)lhs);
        dfcio::fixed_point64<0> b((int64_t)rhs);
        dfcio::fixed_point128<5> c = a / b;

        dfcio::fixed_point128<5> e = dfcio::fixed_divide<5>(lhs, rhs);
        print(e);
        dfcio_assert(c == e, "fixed_point64 instances division result from operator and function and compare in fixed_point128");

    }

    {
        uint32_t lhs = 100000;
        uint32_t rhs = 33;

        dfcio::fixed_point32<0> a((int32_t)lhs);
        dfcio::fixed_point32<0> b((int32_t)rhs);
        dfcio::fixed_point64<5> c = a / b;

        dfcio::fixed_point64<5> e = dfcio::fixed_divide<5>(lhs, rhs);
        dfcio_assert(c == e, "fixed_point64 instances division result from operator and function and compare in fixed_point128");

    }
}

void test_fixedpoint::test_division_by_0()
{
    {
        uint64_t lhs = 10000000;
        uint64_t rhs = 0;

        dfcio::fixed_point64<0> a((int64_t)lhs);
        dfcio::fixed_point64<0> b((int64_t)rhs);

        dfcio::fixed_point128<5> e = dfcio::fixed_divide<5>(lhs, rhs);
        // in order to get rid of unused parameter warning
        e = 0;
        dfcio_assert(false, "should've thrown an error");

    }

 }
