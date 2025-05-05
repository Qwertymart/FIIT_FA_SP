#ifndef MP_OS_FRACTION_H
#define MP_OS_FRACTION_H

#include <big_int.h>
#include <not_implemented.h>
#include <concepts>
#include <numeric>

class fraction final
{

private:

    big_int _numerator;
    big_int _denominator;

    void optimise(); //сокращает дробь

public:

    /** Perfect forwarding ctor
     */
    template<std::convertible_to<big_int> f, std::convertible_to<big_int> s>
    fraction(f &&numerator, s &&denominator)
            : _numerator(std::forward<f>(numerator)), _denominator(std::forward<s>(denominator))
    {
        if (denominator == 0) {
            throw std::invalid_argument("Denominator cannot be zero.");
        }
        optimise();

    }


    fraction(pp_allocator<big_int::value_type> = pp_allocator<big_int::value_type>());

public:

    fraction &operator+=(fraction const &other) &;

    fraction operator+(fraction const &other) const;

    fraction &operator-=(fraction const &other) &;

    fraction operator-(fraction const &other) const;

    fraction &operator*=(fraction const &other) &;

    fraction operator*(fraction const &other) const;

    fraction &operator/=(fraction const &other) &;

    fraction operator/(fraction const &other) const;

    fraction abs() const;

public:


    bool operator==(fraction const &other) const noexcept;

    std::partial_ordering operator<=>(const fraction& other) const noexcept;

public:

    friend std::ostream &operator<<(std::ostream &stream, fraction const &obj);

    friend std::istream &operator>>(std::istream &stream, fraction &obj);

    std::string to_string() const;

public:

    fraction sin(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction cos(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction tg(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction ctg(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction sec(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction cosec(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction arcsin(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction arccos(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction arctg(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction arcctg(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction arcsec(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction arccosec(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction sqrt(const fraction& epsilon) const;
    fraction ln_of_2(const fraction& epsilon) const;

public:

    fraction pow(size_t degree) const;

public:

    fraction root(size_t degree, fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

public:

    fraction log2(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction ln(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

    fraction lg(fraction const &epsilon = fraction(1_bi, 1000000_bi)) const;

};


inline big_int abs(big_int const& value) {
    return value < 0_bi ? -value : value;
}

inline big_int gcd(big_int a, big_int b) {
    while (b != 0_bi) {
        big_int temp = b;
        b = a % b;
        a = temp;
    }
    return a;
}


#endif //MP_OS_FRACTION_H