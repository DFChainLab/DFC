#pragma once
#include <dfciolib/types.h>
#include <dfciolib/print.hpp>

namespace dfcio
{
    /**
    * @defgroup fixedpoint Fixed Point
    * @ingroup mathcppapi
    * @brief 32,64,128,256 bits version of fixed point variables
    *
    * Floating point operations are indeterministic, hence is prevented in smart contract.
    * The smart contract developers should use the appropriate Fixed_Point template class
    * by passing the number to be represented in integer format and the number of decimals
    * required.
    * These template classes also support the arithmetic operations and basic comparison operators
    * @{
    */

    // Some forward declarations
    template <uint8_t Q> struct fixed_point32;
    template <uint8_t Q> struct fixed_point64;
    template <uint8_t Q> struct fixed_point128;

    // Will support fixed_point256 in next release
#if 0
    template <uint8_t Q> struct fixed_point256;
    /**
    * @defgroup Template class for Fixed Point 256 bits representaton
    * @ingroup contractdev
    * @brief Template param Q is the Q factor i.e. number of decimals
    *
    */
    template <uint8_t Q>
    struct fixed_point256
    {
        int128_t val;
        fixed_point256(int256_t v=0) : val(v) {}
        template <uint8_t QR> fixed_point256(const fixed_point256<QR> &r);
        template <uint8_t QR> fixed_point256(const fixed_point128<QR> &r);
        template <uint8_t QR> fixed_point256(const fixed_point64<QR> &r);
        template <uint8_t QR> fixed_point256(const fixed_point32<QR> &r);
        /**
        * Get the integer part of the 64 bit fixed number
        * @brief To get the integer part of the fixed number
        * @return Returns integer part of the fixed number
        *
        * Example:
        * @code
        * fixed64<18> a(1234.455667)
        * std::cout << a.int_part(); // Output: 1234
        * @endcode
        */
        int128_t int_part() const {
            return val >> q;
        }

        /**
        * Get the decimal part of the 64 bit fixed number
        * @brief To get the decimal part of the fixed number
        * @return Returns decimal part of the fixed number
        *
        * Example:
        * @code
        * fixed64<18> a(1234.455667)
        * std::cout << a.decimal_part(); // Output: 45567
        * @endcode
        */
        uint128_t frac_part() const {
            if(!Q) return 0;
            return val << (32-Q);
        }



        template <uint8_t QR> fixed_point256 &operator=(const fixed_point32<QR> &r);
        template <uint8_t QR> fixed_point256 &operator=(const fixed_point64<QR> &r);
        template <uint8_t QR> fixed_point256 &operator=(const fixed_point128<QR> &r);
        template <uint8_t QR> fixed_point256 &operator=(const fixed_point256<QR> &r);
        // Comparison functions
        template <uint8_t QR> bool operator==(const fixed_point256<QR> &r) { return (val == r.val);}
        template <uint8_t QR> bool operator>(const fixed_point256<QR> &r) { return (val > r.val);}
        template <uint8_t QR> bool operator<(const fixed_point256<QR> &r) { return (val < r.val);}
    };
#endif

    /**
    * The template param Q represents the Q Factor i.e number of decimals
    * 
    * @brief 128 bits representation of Fixed Point class.
    *
    * Example:
    * @code
    * fixed_point128<6> a(123232.455667233)
    * fixed_point128<0> a(123424)
    * fixed_point128<18> c = a*b;
    * fixed_point128<24> d = a+b+c;
    * fixed_point128<24> e = b/a;
    * @endcode
    */
    template <uint8_t Q>
    struct fixed_point128
    {
        static_assert(Q < 128, "Maximum number of decimals supported in fixed_point128 is 128 decimals");

        /**
         * @brief Value of the fixed point represented as int128_t
         * 
         * Value of the fixed point represented as int128_t
         */
        int128_t val;

        /**
        * Various constructors for fixed_point128. Can create fixed_point128 instance from an int128_t, fixed_point128,64,32 instance
        * 
        * @brief Various constructors for fixed_point128
        *
        * Example:
        * @code
        * fixed_point64<18> a(1234.455667);
        * fixed_point128<3> b(a);
        * fixed_point32<6> b(13324.32323);
        * fixed_point128<5> c(a);
        * @endcode
        */
        
        /**
         * Construct a new fixed point128 object from int128_t
         * 
         * @brief Construct a new fixed point128 object
         * @param v - int128_t representation of the fixed point value
         */
        fixed_point128(int128_t v=0) : val(v) {}

         /**
         * Construct a new fixed point128 object from another fixed_point128
         * 
         * @brief Construct a new fixed point128 object from another fixed_point128
         * @param r - Another fixed_point128 as source 
         */
        template <uint8_t qr> fixed_point128(const fixed_point128<qr> &r);

        /**
         * Construct a new fixed point128 object from another fixed_point64
         * 
         * @brief Construct a new fixed point128 object from another fixed_point64
         * @param r -fixed_point64 as source 
         */
        template <uint8_t qr> fixed_point128(const fixed_point64<qr> &r);

        /**
         * Construct a new fixed point128 object from another fixed_point32
         * 
         * @brief Construct a new fixed point128 object from another fixed_point32
         * @param r -fixed_point32 as source 
         */
        template <uint8_t qr> fixed_point128(const fixed_point32<qr> &r);

        /**
        * Get the integer part of the 64 bit fixed number
        * 
        * @brief To get the integer part of the fixed number
        * @return Returns integer part of the fixed number
        *
        * Example:
        * @code
        * fixed_point64<5> a(1234.455667)
        * std::cout << a.int_part(); // Output: 1234
        * @endcode
        */
        int128_t int_part() const {
            return val >> Q;
        }

        /**
        * Get the decimal part of the 64 bit fixed number
        * 
        * @brief To get the decimal part of the fixed number
        * @return Returns decimal part of the fixed number
        *
        * Example:
        * @code
        * fixed_point128<3> a(1234.455667)
        * std::cout << a.decimal_part(); // Output: 455
        * @endcode
        */
        uint128_t frac_part() const {
            if(!Q) return 0;
            return uint128_t(val << (32-Q));
        }

        /**
         * Prints the fixed point value
         * 
         * @brief Prints the fixed point value
         */
        void print() const {
           uint128_t ip((uint128_t)int_part());
           uint128_t fp(frac_part());
           printui128(&ip);
           prints(".");
           printui128(&fp);
        }

        // Various assignment operators
        /**
         * Assignment operator. Assign fixed_point32 to fixed_point128
         * 
         * @brief Assignment operator
         * @tparam qr - Precision of the source
         * @param r - Source
         * @return fixed_point128& - Reference to this object
         */
        template <uint8_t qr> fixed_point128 &operator=(const fixed_point32<qr> &r);
         /**
         * Assignment operator. Assign fixed_point32 to fixed_point64
         * 
         * @brief Assignment operator
         * @tparam qr - Precision of the source
         * @param r - Source
         * @return fixed_point128& - Reference to this object
         */
        template <uint8_t qr> fixed_point128 &operator=(const fixed_point64<qr> &r);
         /**
         * Assignment operator. Assign fixed_point32 to fixed_point32
         * 
         * @brief Assignment operator
         * @tparam qr - Precision of the source
         * @param r - Source
         * @return fixed_point128& - Reference to this object
         */
        template <uint8_t qr> fixed_point128 &operator=(const fixed_point128<qr> &r);

        // Comparison functions
        /**
         * Equality operator
         * 
         * @brief Equality operator
         * @tparam qr - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t qr> bool operator==(const fixed_point128<qr> &r) { return (val == r.val);}

         /**
         * Greater than operator
         * 
         * @brief Greater than operator
         * @tparam qr - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t qr> bool operator>(const fixed_point128<qr> &r) { return (val > r.val);}

         /**
         * Less than operator
         * 
         * @brief Less than operator
         * @tparam qr - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t qr> bool operator<(const fixed_point128<qr> &r) { return (val < r.val);}
    };


    /**
    * @brief 64 bits representation of Fixed Point class.
    *
    * Example:
    * @code
    * fixed_point64<6> a(123232.455667233)
    * fixed_point64<0> a(123424)
    * fixed_point64<18> c = a*b;
    * fixed_point64<24> d = a+b+c;
    * fixed_point64<24> e = b/a;
    * @endcode
    */
    template <uint8_t Q>
    struct fixed_point64
    {
        static_assert(Q < 128, "Maximum number of decimals supported in fixed_point64 is 128 decimals");

        /**
         * @brief Value of the fixed point represented as int64_t
         * 
         * Value of the fixed point represented as int64_t
         */
        int64_t val;

        /**
         * Construct a new fixed point64 object from int64_t
         * 
         * @brief Construct a new fixed point64 object
         * @param v - int64_t representation of the fixed point value
         */
        fixed_point64(int64_t v=0) : val(v) {}

        /**
         * Construct a new fixed point64 object from another fixed_point64
         * 
         * @brief Construct a new fixed point64 object from another fixed_point64
         * @param r - Another fixed_point64 as source 
         */
        template <uint8_t QR> fixed_point64(const fixed_point64<QR> &r);

        /**
         * Construct a new fixed point64 object from another fixed_point32
         * 
         * @brief Construct a new fixed point64 object from another fixed_point32
         * @param r - fixed_point64 as source 
         */
        template <uint8_t QR> fixed_point64(const fixed_point32<QR> &r);

        /**
        * Get the integer part of the 64 bit fixed number
        * @brief To get the integer part of the fixed number
        * @return Returns integer part of the fixed number
        *
        * Example:
        * @code
        * fixed_point64<18> a(1234.455667)
        * std::cout << a.int_part(); // Output: 1234
        * @endcode
        */
        int64_t int_part() const {
            return val >> Q;
        }

        /**
        * Get the decimal part of the 64 bit fixed number
        * @brief To get the decimal part of the fixed number
        * @return Returns decimal part of the fixed number
        *
        * Example:
        * @code
        * fixed64<3> a(1234.455667)
        * std::cout << a.decimal_part(); // Output: 455
        * @endcode
        */
        uint64_t frac_part() const {
            if(!Q) return 0;
            return uint64_t(val << (32-Q));
        }

        /**
         * Prints the fixed point value
         * 
         * @brief Prints the fixed point value
         */
        void print() const {
           printi(int_part());
           prints(".");
           printi128(frac_part());
        }

        // Various assignment operators
        /**
         * Assignment operator. Assign fixed_point32 to fixed_point64
         * 
         * @brief Assignment operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return fixed_point64& - Reference to this object
         */
        template <uint8_t QR> fixed_point64 &operator=(const fixed_point32<QR> &r);

        /**
         * Assignment operator. Assign fixed_point64 to fixed_point64
         * 
         * @brief Assignment operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return fixed_point64& - Reference to this object
         */
        template <uint8_t QR> fixed_point64 &operator=(const fixed_point64<QR> &r);

        // Arithmetic operations
        /**
         * Addition operator
         * 
         * @brief Addition operator
         * @tparam QR - Precision of the second addend
         * @param r - Second addend
         * @return - The result of addition
         */
        template <uint8_t QR> fixed_point64< (Q>QR)?Q:QR > operator+(const fixed_point64<QR> &r) const;

        /**
         * Subtraction operator
         * 
         * @brief Subtraction operator
         * @tparam QR - Precision of the minuend
         * @param r - Minuend
         * @return - The result of subtraction
         */
        template <uint8_t QR> fixed_point64< (Q>QR)?Q:QR > operator-(const fixed_point64<QR> &r) const;

        // product and division of two fixed_point64 instances will be fixed_point128
        // The total number of decimals will be the max
        /**
         * Multiplication operator
         * 
         * @brief Multiplication operator
         * @tparam QR - Precision of the multiplier
         * @param r - Multiplier
         * @return - The result of multiplication
         */
        template <uint8_t QR> fixed_point128<Q+QR> operator*(const fixed_point64<QR> &r) const;

        /**
         * Division operator
         * 
         * @brief Division operator
         * @tparam QR - Precision of the divisor
         * @param r - Divisor
         * @return - The result of division
         */
        template <uint8_t QR> fixed_point128<Q+64-QR> operator/(const fixed_point64<QR> &r) const;

        // Comparison functions
        /**
         * Equality operator
         * 
         * @brief Equality operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t QR> bool operator==(const fixed_point64<QR> &r) { return (val == r.val);}

        /**
         * Greater than operator
         * 
         * @brief Greater than operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t QR> bool operator>(const fixed_point64<QR> &r) { return (val > r.val);}

        /**
         * Less than operator
         * 
         * @brief Less than operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t QR> bool operator<(const fixed_point64<QR> &r) { return (val < r.val);}
    };

    /**
     * @brief 32 bits representation of Fixed Point class.
     *
     * This class is implemented to to replace the floating point variables
     * It can resolve floating point undetermenistic related issues
     *
     * Example:
     * @code
     *
     * fixed_point32<17> b(9.654);
     * fixed_point32<18> c = a*b;
     * fixed_point32<24> d = a+b+c;
     * fixed_point32<24> e = b/a;
     * @endcode
     *
     */
    // fixed_point 32 bit version. The template param 'q' is the scale factor
    template <uint8_t Q>
    struct fixed_point32
    {
        static_assert(Q < 128, "Maximum number of decimals supported in fixed_point32 is 128 decimals");
        
        /**
         * @brief Value of the fixed point represented as int32_t
         * 
         * Value of the fixed point represented as int32_t
         */
        int32_t val;

        /**
         * Construct a new fixed point32 object from another fixed_point32
         * 
         * @brief Construct a new fixed point32 object from another fixed_point32
         * @param r - Another fixed_point32 as source 
         */    
        template <uint8_t QR> fixed_point32(const fixed_point32<QR> &r);

        /**
         * Construct a new fixed point32 object from another fixed_point64. It will be truncated.
         * 
         * @brief Construct a new fixed point32 object from another fixed_point64
         * @param r - Another fixed_point32 as source 
         */
        template <uint8_t QR> fixed_point32(const fixed_point64<QR> &r);
       

        /**
         * Construct a new fixed point32 object from int32_t
         * 
         * @brief Construct a new fixed point32 object
         * @param param - int32_t representation of the fixed point value
         */
        fixed_point32(int32_t param=0) : val(param) {}

        // translates given double variable to the int32 based on the scale factor
        // fixed_point32(double d=0) : val(d * (1<<q) ) { }
        /*
        double to_double() const {
            return double(val) / (1<<q);
        }
        */

        /**
        * Get the integer part of the 64 bit fixed number
        * @brief To get the integer part of the fixed number
        * @return Returns integer part of the fixed number
        *
        * Example:
        * @code
        * fixed_point32<18> a(1234.455667)
        * std::cout << a.int_part(); // Output: 1234
        * @endcode
        */
        int32_t int_part() const {
            return val >> Q;
        }
        uint32_t frac_part() const {
            if(!Q) return 0;
            return uint32_t(val << (32-Q));
        }

        /**
         * Prints the fixed point value
         * 
         * @brief Prints the fixed point value
         */
        void print() const {
           printi(int_part());
           prints(".");
           printi128(frac_part());
        }

        // Various assignment operators
        /**
         * Assignment operator. Assign fixed_point32 to fixed_point32
         * 
         * @brief Assignment operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return fixed_point32& - Reference to this object
         */
        template <uint8_t QR> fixed_point32 &operator=(const fixed_point32<QR> &r);

        /**
         * Assignment operator. Assign fixed_point64 to fixed_point32
         * 
         * @brief Assignment operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return fixed_point32& - Reference to this object
         */
        template <uint8_t QR> fixed_point32 &operator=(const fixed_point64<QR> &r);

        /**
         * Addition operator
         * 
         * @brief Addition operator
         * @tparam QR - Precision of the second addend
         * @param r - Second addend
         * @return - The result of addition
         */
        template <uint8_t QR> fixed_point32< (Q>QR)?Q:QR > operator+(const fixed_point32<QR> &r) const;

        /**
         * Subtraction operator
         * 
         * @brief Subtraction operator
         * @tparam QR - Precision of the minuend
         * @param r - Minuend
         * @return - The result of subtraction
         */
        template <uint8_t QR> fixed_point32< (Q>QR)?Q:QR > operator-(const fixed_point32<QR> &r) const;

        // productd of to fixed_point32 instances will be fixed_point64
        /**
         * Multiplication operator
         * 
         * @brief Multiplication operator
         * @tparam QR - Precision of the multiplier
         * @param r - Multiplier
         * @return - The result of multiplication
         */
        template <uint8_t QR> fixed_point64<Q+QR> operator*(const fixed_point32<QR> &r) const;

        /**
         * Division operator
         * 
         * @brief Division operator
         * @tparam QR - Precision of the divisor
         * @param r - Divisor
         * @return - The result of division
         */
        template <uint8_t QR> fixed_point64<Q+32-QR> operator/(const fixed_point32<QR> &r) const;

        // Comparison functions
        /**
         * Equality operator
         * 
         * @brief Equality operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t QR> bool operator==(const fixed_point32<QR> &r) { return (val == r.val);}

        /**
         * Greater than operator
         * 
         * @brief Greater than operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t QR> bool operator>(const fixed_point32<QR> &r) { return (val > r.val);}

        /**
         * Less than operator
         * 
         * @brief Less than operator
         * @tparam QR - Precision of the source
         * @param r - Source
         * @return true - if equal
         * @return false - otherwise
         */
        template <uint8_t QR> bool operator<(const fixed_point32<QR> &r) { return (val < r.val);}
    };


   /// @} fixedpoint

    // Helper functions
    template<typename T>
    T assignHelper(T rhs_val, uint8_t q, uint8_t qr)
    {
        T result = (q > qr) ? rhs_val << (q-qr) : rhs_val >> (qr-q);
        return result;
    }


#if 0
    // fixed_point256 methods
    template<uint32_t q> template<uint32_t qr>
    fixed_point256<q>::fixed_point256(const fixed_point256<qr> &r) {
        val = assignHelper<int256_t>(r.val, q, qr);
    }

    template<uint32_t q> template<uint32_t qr>
    fixed_point256<q>::fixed_point256(const fixed_point128<qr> &r) {
        val = assignHelper<int256_t>(r.val, q, qr);
    }

    template<uint32_t q> template<uint32_t qr>
    fixed_point256<q>::fixed_point256(const fixed_point64<qr> &r) {
        val = assignHelper<int256_t>(r.val, q, qr);
    }

    template<uint32_t q> template <uint32_t qr>
    fixed_point256<q>::fixed_point256(const fixed_point32<qr> &r) {
        val = assignHelper<int256_t>(r.val, q, qr);
    }
#endif

    // fixed_point128 methods
    template<uint8_t Q> template<uint8_t QR>
    fixed_point128<Q>::fixed_point128(const fixed_point128<QR> &r) {
        val = assignHelper<int128_t>(r.val, Q, QR);
    }

    template<uint8_t Q> template<uint8_t QR>
    fixed_point128<Q>::fixed_point128(const fixed_point64<QR> &r) {
        val = assignHelper<int128_t>(r.val, Q, QR);
    }

    template<uint8_t Q> template <uint8_t QR>
    fixed_point128<Q>::fixed_point128(const fixed_point32<QR> &r) {
        val = assignHelper<int128_t>(r.val, Q, QR);
    }


    // fixed_point64 methods
    template<uint8_t Q> template<uint8_t QR>
    fixed_point64<Q>::fixed_point64(const fixed_point64<QR> &r) {
        val = assignHelper<int64_t>(r.val, Q, QR);
    }

    template<uint8_t Q> template <uint8_t QR>
    fixed_point64<Q>::fixed_point64(const fixed_point32<QR> &r) {
        val = assignHelper<int32_t>(r.val, Q, QR);
    }

    /**
    * @brief Addition between two fixed_point64 variables and the result goes to fixed_point64
    *
    * Addition between two fixed_point64 variables
    * Number of decimal on result will be max of decimals of lhs and rhs
    */
    template<uint8_t Q> template<uint8_t QR>
    fixed_point64< (Q>QR)?Q:QR > fixed_point64<Q>::operator+(const fixed_point64<QR> &rhs) const
    {
        // if the scaling factor for both are same, no need to make any intermediate objects except the result
        if(Q == QR)
        {
            return fixed_point64<Q>(val + rhs.val);
        }
        return fixed_point64<(Q>QR)?Q:QR>(
            fixed_point64<(Q>QR)?Q:QR>( *this ).val +
            fixed_point64<(Q>QR)?Q:QR>( rhs ).val
        );
    }

    /**
    * @brief Subtraction between two fixed_point64 variables and the result goes to fixed_point64
    *
    * Subtraction between two fixed_point64 variables
    * Number of decimal on result will be max of decimals of lhs and rhs
    */
    template<uint8_t Q> template<uint8_t QR>
    fixed_point64< (Q>QR)?Q:QR > fixed_point64<Q>::operator-(const fixed_point64<QR> &rhs) const
    {
        // if the scaling factor for both are same, no need to make any intermediate objects except the result
        if(Q == QR)
        {
            return fixed_point64<Q>(val - rhs.val);
        }
        return fixed_point64<(Q>QR)?Q:QR>(
            fixed_point64<(Q>QR)?Q:QR>( *this ).val -
            fixed_point64<(Q>QR)?Q:QR>( rhs ).val
        );
    }

    /**
    * @brief Multiplication operator for fixed_point64. The result goes to fixed_point64
    *
    * Multiplication operator for fixed_point64. The result goes to fixed_point128
    * Number of decimal on result will be sum of number of decimals of lhs and rhs
    *
    * Example:
    * @code
    * fixed_point128<33> result = fixed_point64<0>(131313) / fixed_point64<0>(2323)
    * @endcode
    */
    template<uint8_t Q> template <uint8_t QR>
    fixed_point128<Q+QR> fixed_point64<Q>::operator*(const fixed_point64<QR> &r) const {
        return fixed_point128<Q+QR>(int128_t(val)*r.val);
    }

    /**
    * @brief Division of two fixed_point64 result will be stored in fixed_point128
    *
    * Division operator for fixed_point64
    *
    * Example:
    * @code
    * fixed_point128<33> result = fixed_point64<0>(131313) / fixed_point64<0>(2323)
    * @endcode
    */
    template <uint8_t Q> template <uint8_t QR>
    fixed_point128<Q+64-QR> fixed_point64<Q>::operator/(const fixed_point64<QR> &r) const {
        // std::cout << "Performing division on " << val << ", with " << q << " precision / " << r.val << ", with " << qr << " precision. Result precision " << ((q>qr) ? q:qr) << std::endl;
        // Convert val to 128 bit by additionally shifting 64 bit and take the result to 128bit
        // Q(X+64-Y) = Q(X+64) / Q(Y)
       dfcio_assert( !(r.int_part() == 0 && r.frac_part() == 0), "divide by zero" );
        return fixed_point128<Q+64-QR>((int128_t(val)<<64)/r.val);
    }

    // fixed_point32 methods
    template<uint8_t Q> template <uint8_t QR>
    fixed_point32<Q>::fixed_point32(const fixed_point32<QR> &r) {
        val = assignHelper<int32_t>(r.val, Q, QR);
    }

    template<uint8_t Q> template <uint8_t QR>
    fixed_point32<Q>::fixed_point32(const fixed_point64<QR> &r) {
        val = assignHelper<int32_t>(r.val, Q, QR);
    }

    template<uint8_t Q> template <uint8_t QR>
    fixed_point32<Q> &fixed_point32<Q>::operator=(const fixed_point32<QR> &r) {
        val = assignHelper<int32_t>(r.val, Q, QR);
    }

    template<uint8_t Q> template <uint8_t QR>
    fixed_point32<Q> &fixed_point32<Q>::operator=(const fixed_point64<QR> &r) {
        val = assignHelper<int32_t>(r.val, Q, QR);
    }

    /**
    * @brief Addition between two fixed_point32 variables and the result goes to fixed_point32
    *
    * Addition between two fixed_point32 variables
    * Number of decimal on result will be max of decimals of lhs and rhs
    *
    */
    template<uint8_t Q> template<uint8_t QR>
    fixed_point32< (Q>QR)?Q:QR > fixed_point32<Q>::operator+(const fixed_point32<QR> &rhs) const
    {
        // if the scaling factor for both are same, no need to make any intermediate objects except the result
        if(Q == QR)
        {
            return fixed_point32<Q>(val + rhs.val);
        }
        return fixed_point32<(Q>QR)?Q:QR>(
            fixed_point32<(Q>QR)?Q:QR>( *this ).val +
            fixed_point32<(Q>QR)?Q:QR>( rhs ).val
        );
    }

    /**
    * @brief Subtraction between two fixed_point32 variables and the result goes to fixed_point32
    *
    * Subtraction between two fixed_point32 variables
    * Number of decimal on result will be max of decimals of lhs and rhs
    *
    */
    template<uint8_t Q> template<uint8_t QR>
    fixed_point32< (Q>QR)?Q:QR > fixed_point32<Q>::operator-(const fixed_point32<QR> &rhs) const
    {
        // if the scaling factor for both are same, no need to make any intermediate objects except the result
        if(Q == QR)
        {
            return fixed_point32<Q>(val - rhs.val);
        }
        return fixed_point32<(Q>QR)?Q:QR>(
            fixed_point32<(Q>QR)?Q:QR>( *this ).val -
            fixed_point32<(Q>QR)?Q:QR>( rhs ).val
        );
    }

    /**
    * @brief Multiplication operator for fixed_point32. The result goes to fixed_point64
    *
    * Multiplication operator for fixed_point32. The result goes to fixed_point64
    * Number of decimal on result will be sum of number of decimals of lhs and rhs
    *
    * Example:
    * @code
    * fixed_point64<33> result = fixed_point32<0>(131313) / fixed_point32<0>(2323)
    * @endcode
    */
    template<uint8_t Q> template <uint8_t QR>
    fixed_point64<Q+QR> fixed_point32<Q>::operator*(const fixed_point32<QR> &r) const {
        return fixed_point64<Q+QR>(int64_t(val)*r.val);
    }

    /**
    * @brief Division of two fixed_point32 result will be stored in fixed_point64
    *
    * Division operator for fixed_point32
    *
    * Example:
    * @code
    * fixed_point64<33> result = fixed_point32<0>(131313) / fixed_point32<0>(2323)
    * @endcode
    */
    template <uint8_t Q> template <uint8_t QR>
    fixed_point64<Q+32-QR> fixed_point32<Q>::operator/(const fixed_point32<QR> &r) const {
        // Convert val into 64 bit and perform the division
        // Q(X+32-Y) = Q(X+32) / Q(Y)
        dfcio_assert( !(r.int_part() == 0 && r.frac_part() == 0), "divide by zero" );
        return fixed_point64<Q+32-QR>((int64_t(val)<<32)/r.val);
    }

    /**
    * @brief Wrapper function for dividing two unit64 variable and stores result in fixed_point64
    *
    * Wrapper function for dividing two unit32 variable and stores result in fixed_point64
    *
    * Example:
    * @code
    * fixed_point64<33> result = fixed_divide(131313, 2323)
    * @endcode
    */
    template <uint8_t Q>
    fixed_point64<Q> fixed_divide(uint32_t lhs, uint32_t rhs)
    {

        dfcio_assert( rhs != 0, "divide by zero" );
        fixed_point64<Q> result = fixed_point32<0>((int32_t)lhs) / fixed_point32<0>((int32_t)rhs);
        return result;
    }

    /**
    * @brief Wrapper function for dividing two unit64 variable and stores result in fixed_point128
    * Wrapper function for dividing two unit64 variable and stores result in fixed_point128
    *
    * Example:
    * @code
    * fixed_point128<33> result = fixed_divide(131313, 2323)
    * @endcode
    */

    template <uint8_t Q>
    fixed_point128<Q> fixed_divide(uint64_t lhs, uint64_t rhs)
    {

        dfcio_assert( rhs != 0, "divide by zero" );
        fixed_point128<Q> result = fixed_point64<0>((int32_t)lhs) / fixed_point64<0>((int32_t)rhs);
        return fixed_point128<Q>(result);
    }


};
