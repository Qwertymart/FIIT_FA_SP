//
// Created by Des Caldnd on 5/27/2024.
//

#include "../include/big_int.h"
#include <ranges>
#include <exception>
#include <string>
#include <sstream>
#include <cmath>
#include <algorithm>

std::strong_ordering big_int::operator<=>(const big_int &other) const noexcept
{
    // Сравнение знаков: положительное больше отрицательного
    if (_sign != other._sign) {
        return _sign ? std::strong_ordering::greater : std::strong_ordering::less;
    }

    // Если оба нули, считаем их равными
    if (_digits.empty() && other._digits.empty()) {
        return std::strong_ordering::equal;
    }

    // Сравнение по количеству цифр
    if (_digits.size() != other._digits.size()) {
        if (_sign) {
            return _digits.size() < other._digits.size()
                   ? std::strong_ordering::less
                   : std::strong_ordering::greater;
        } else {
            return _digits.size() < other._digits.size()
                   ? std::strong_ordering::greater
                   : std::strong_ordering::less;
        }
    }

    // Если размер совпадает — сравниваем цифры начиная с самой значимой (обратный порядок)
    for (std::size_t i = _digits.size(); i-- > 0;) {
        if (_digits[i] != other._digits[i]) {
            if (_sign) {
                return _digits[i] < other._digits[i]
                       ? std::strong_ordering::less
                       : std::strong_ordering::greater;
            } else {
                return _digits[i] < other._digits[i]
                       ? std::strong_ordering::greater
                       : std::strong_ordering::less;
            }
        }
    }

    return std::strong_ordering::equal;
}

big_int::operator bool() const noexcept
{
    return !_digits.empty() && !(_digits.size() == 1 && _digits[0] == 0);
}

big_int &big_int::operator++() &
{
    return operator+=(1);
}


big_int big_int::operator++(int)
{
    auto tmp = *this;
    ++(*this);
    return tmp;
}

big_int &big_int::operator--()&
{
    return operator-=(1);
}


big_int big_int::operator--(int)
{
    auto tmp = *this;
    --(*this);
    return tmp;
}

big_int &big_int::operator+=(const big_int &other)&
{
    return plus_assign(other);
}

big_int &big_int::operator-=(const big_int &other)&
{
    return minus_assign(other);
}


big_int big_int::operator+(const big_int &other) const {
    if (_sign == other._sign) {
        big_int result = *this;
        result.plus_assign(other);
        return result;
    } else {
        if (compare_absolute(*this, other) >= 0) {
            big_int result = *this;
            result.minus_assign(other);
            return result;
        } else {
            big_int result = other;
            result.minus_assign(*this);
            result._sign = other._sign;
            return result;
        }
    }
}


big_int big_int::operator-(const big_int &other) const {
    if (_sign != other._sign) {
        big_int result = *this;
        result.plus_assign(other);
        return result;
    } else {
        if (compare_absolute(*this, other) >= 0) {
            big_int result = *this;
            result.minus_assign(other);
            return result;
        } else {
            big_int result = other;
            result.minus_assign(*this);
            result._sign = !other._sign;
            return result;
        }
    }
}


int big_int::compare_absolute(const big_int& a, const big_int& b) const {
    if (a._digits.size() != b._digits.size()) {
        return a._digits.size() > b._digits.size() ? 1 : -1;
    }
    for (size_t i = a._digits.size(); i-- > 0; ) {
        if (a._digits[i] != b._digits[i]) {
            return a._digits[i] > b._digits[i] ? 1 : -1;
        }
    }
    return 0;
}

big_int big_int::operator*(const big_int &other) const
{
    auto tmp = *this;
    return tmp *= other;
}

big_int big_int::operator/(const big_int &other) const
{
    auto tmp = *this;
    return tmp /= other;
}

big_int big_int::operator%(const big_int &other) const
{
    auto tmp = *this;
    return tmp %= other;
}

big_int big_int::operator&(const big_int &other) const
{
    auto tmp = *this;
    return tmp &= other;
}
big_int big_int::operator|(const big_int &other) const
{
    auto tmp = *this;
    return tmp |= other;
}

big_int big_int::operator^(const big_int &other) const
{
    auto tmp = *this;
    return tmp ^= other;
}

big_int big_int::operator<<(size_t shift) const
{
    auto tmp = *this;
    return tmp <<= shift;
}

big_int big_int::operator>>(size_t shift) const
{
    auto tmp = *this;
    return tmp >>= shift;
}

big_int &big_int::operator%=(const big_int &other) &
{
    return operator-=((*this / other) * other);
}

big_int big_int::operator~() const
{
    auto res = *this;
    res._sign = !res._sign;
    for (auto& num : res._digits)
    {
        num = ~num;
    }
    while (res._digits.size() > 1 && res._digits.back() == 0) {
        res._digits.pop_back();
    }
    return res;
}

big_int &big_int::operator&=(const big_int &other) &
{
    if (!_sign && other._sign)
        _sign = true;

    for(size_t i = 0; i < _digits.size(); ++i)
        _digits[i] &= i < other._digits.size() ? other._digits[i] : 0;

    while (_digits.size() > 1 && _digits.back() == 0) {
        _digits.pop_back();
    }
    return *this;
}

big_int &big_int::operator|=(const big_int &other) &
{
    if (_sign && !other._sign)
        _sign = false;

    if (_digits.size() < other._digits.size())
        _digits.resize(other._digits.size(), 0);

    for(size_t i = 0; i < _digits.size(); ++i)
        _digits[i] |= other._digits[i];

    while (_digits.size() > 1 && (_digits.back() == 0)) {
        _digits.pop_back();
    }
    return *this;
}

big_int &big_int::operator^=(const big_int &other) &
{
    if (_sign != other._sign)
        _sign = true;
    else
        _sign = false;

    if (_digits.size() < other._digits.size())
        _digits.resize(other._digits.size(), 0);

    for(size_t i = 0; i < _digits.size(); ++i)
        _digits[i] ^= other._digits[i];

    while (_digits.size() > 1 && (_digits.back() == 0)) {
        _digits.pop_back();
    }
    return *this;
}

big_int &big_int::operator<<=(size_t shift) &
{
    if (shift / (8 * sizeof(unsigned int)) > 0)
    {
        size_t n = shift / (8 * sizeof(unsigned int));

        std::vector<unsigned int> vec(n, 0);

        _digits.insert(_digits.begin(), vec.begin(), vec.end());

        shift %= 8 * sizeof(unsigned int);
    }

    if (shift == 0)
        return *this;

    unsigned int c = 0;

    for(auto & num : _digits)
    {
        auto tmp = num;
        num = (num << shift) | c;
        c = tmp >> (8 * sizeof(unsigned int) - shift);
    }

    if (c != 0)
        _digits.push_back(c);

    while (_digits.size() > 1 && (_digits.back() == 0)) {
        _digits.pop_back();
    }
    return *this;
}

big_int &big_int::operator>>=(size_t shift) &
{
    if (shift / (8 * sizeof(unsigned int)) > 0)
    {
        size_t n = shift / (8 * sizeof(unsigned int));

        if (n > _digits.size())
        {
            _sign = true;
            _digits.clear();
            return *this;
        }

        _digits.erase(_digits.begin(), _digits.begin() + n);

        shift %= 8 * sizeof(unsigned int);
    }

    if (shift == 0)
        return *this;


    unsigned int c = 0;

    for(auto& num : std::views::reverse(_digits))
    {
        auto tmp = num;
        num = (num >> shift) | c;
        c =  tmp << (8 * sizeof(unsigned int) - shift);
    }

    while (_digits.size() > 1 && (_digits.back() == 0)) {
        _digits.pop_back();
    }
    return *this;
}

big_int &big_int::plus_assign(const big_int &other, size_t shift) & {
    if (!other) return *this;

    const size_t part_bits = sizeof(unsigned int) * 4;
    const unsigned int low_mask = (1u << part_bits) - 1;//маска младших битов, для юинт_16 -> 0xFFFF

    size_t total_size = std::max(_digits.size(), other._digits.size() + shift);
    _digits.resize(total_size + 1, 0);

    unsigned int carry = 0;

    for (size_t i = 0; i < other._digits.size(); ++i) {
        size_t idx = i + shift;

        unsigned int a = _digits[idx];
        unsigned int b = other._digits[i];

        unsigned int a_low = a & low_mask;
        unsigned int a_high = a >> part_bits;

        unsigned int b_low = b & low_mask;
        unsigned int b_high = b >> part_bits;

        unsigned int res_low = a_low + b_low + (carry & 1);
        carry = res_low >> part_bits;

        res_low &= low_mask;

        unsigned int res_high = a_high + b_high + carry;
        carry = res_high >> part_bits;

        res_high &= low_mask;

        _digits[idx] = (res_high << part_bits) | res_low;
    }

    for (size_t i = other._digits.size() + shift; carry != 0 && i < _digits.size(); ++i) {
        unsigned int a = _digits[i];
        unsigned int a_low = a & low_mask;
        unsigned int a_high = a >> part_bits;

        unsigned int res_low = a_low + (carry & 1);
        carry = res_low >> part_bits;

        res_low &= low_mask;

        unsigned int res_high = a_high + carry;
        carry = res_high >> part_bits;

        res_high &= low_mask;

        _digits[i] = (res_high << part_bits) | res_low;
    }

    while (_digits.size() > 1 && _digits.back() == 0)
        _digits.pop_back();

    return *this;
}


big_int &big_int::minus_assign(const big_int &other, size_t shift) & {
    if (!other) return *this;

    const size_t part_bits = sizeof(unsigned int) * 4;
    const unsigned int low_mask = (1u << part_bits) - 1;

    size_t total_size = std::max(_digits.size(), other._digits.size() + shift);
    _digits.resize(total_size, 0);

    unsigned int borrow = 0;

    for (size_t i = 0; i < other._digits.size(); ++i) {
        size_t idx = i + shift;

        unsigned int a = _digits[idx];
        unsigned int b = other._digits[i];

        unsigned int a_low = a & low_mask;
        unsigned int a_high = a >> part_bits;

        unsigned int b_low = b & low_mask;
        unsigned int b_high = b >> part_bits;

        unsigned int tmp_low = a_low - b_low - (borrow & 1);
        borrow = (a_low < b_low + (borrow & 1)) ? 1 : 0;
        tmp_low &= low_mask;

        unsigned int tmp_high = a_high - b_high - borrow;
        borrow = (a_high < b_high + borrow) ? 1 : 0;
        tmp_high &= low_mask;

        _digits[idx] = (tmp_high << part_bits) | tmp_low;
    }

    for (size_t i = other._digits.size() + shift; borrow != 0 && i < _digits.size(); ++i) {
        unsigned int a = _digits[i];

        unsigned int a_low = a & low_mask;
        unsigned int a_high = a >> part_bits;

        unsigned int tmp_low = a_low - (borrow & 1);
        borrow = (a_low < (borrow & 1)) ? 1 : 0;
        tmp_low &= low_mask;

        unsigned int tmp_high = a_high - borrow;
        borrow = (a_high < borrow) ? 1 : 0;
        tmp_high &= low_mask;

        _digits[i] = (tmp_high << part_bits) | tmp_low;
    }

    while (_digits.size() > 1 && _digits.back() == 0)
        _digits.pop_back();

    return *this;
}



big_int &big_int::operator*=(const big_int &other) &
{
    return multiply_assign(other, multiplication_rule::Karatsuba);
}

big_int &big_int::operator/=(const big_int &other) &
{
    return divide_assign(other, division_rule::trivial);
}

std::string big_int::to_string() const
{
    if(_digits.empty() || (_digits.size() == 1 && _digits[0] == 0))
        return "0";

    std::stringstream res;
    auto tmp = *this;

    bool sign = tmp._sign;

    tmp._sign = true;

    while (tmp > 0_bi)
    {
        auto val = tmp % 10_bi;
        tmp /= 10;
        res << char('0' + (val._digits.empty() ? 0 : val._digits[0]));
    }

    if (!sign)
    {
        res << '-';
    }

    std::string d = res.str();
    std::reverse(d.begin(), d.end());
    return d;
}

std::ostream &operator<<(std::ostream &stream, const big_int &value)
{
    stream << value.to_string();
    return stream;
}

std::istream &operator>>(std::istream &stream, big_int &value)
{
    std::string input;
    stream >> input;
    value = big_int(input);
    return stream;
}

bool big_int::operator==(const big_int &other) const noexcept
{
    return _sign == other._sign && _digits == other._digits;
}

big_int::big_int(const std::vector<unsigned int, pp_allocator<unsigned int>> &digits, bool sign)
:_digits(digits), _sign(sign)
{
}

big_int::big_int(std::vector<unsigned int, pp_allocator<unsigned int>> &&digits, bool sign) noexcept
:_sign(sign), _digits(std::move(digits))
{
}


big_int::big_int(const std::string &num, unsigned int radix, pp_allocator<unsigned int> alloc)
        : _sign(true), _digits(alloc)
{
    if (radix < 2 || radix > 36) {
        throw std::invalid_argument("Radix must be in the range [2, 36], but is " + std::to_string(radix));
    }

    auto it = num.begin();
    auto end = num.end();

    if (it != end && (*it == '-' || *it == '+')) {
        _sign = (*it == '+');
        ++it;
    }

    if (it == end) {
        throw std::invalid_argument("Invalid number format: empty string after sign.");
    }

    big_int current_value(0, alloc);

    for (; it != end; ++it) {
        char sym = *it;
        unsigned int digit_value = (sym >= '0' && sym <= '9') ? sym - '0' :
                                   (sym >= 'a' && sym <= 'z') ? sym - 'a' + 10 :
                                   (sym >= 'A' && sym <= 'Z') ? sym - 'A' + 10 : radix;

        if (digit_value >= radix) {
            throw std::invalid_argument("Invalid character '" + std::string(1, sym) + "' for radix " + std::to_string(radix));
        }

        current_value *= big_int(radix, alloc);
        current_value += big_int(digit_value, alloc);
    }

    if (current_value == big_int(0, alloc)) {
        _sign = true;
    }

    _digits = std::move(current_value._digits);
}

big_int::big_int(pp_allocator<unsigned int> alloc)
:_sign(true), _digits(alloc)
{
    _digits.push_back(0);
}

big_int &big_int::multiply_assign(const big_int &other, big_int::multiplication_rule rule) &
{
    if (!*this || !other) {
        *this = big_int(0);
        return *this;
    }

    big_int result((pp_allocator<unsigned int>()));

    switch (rule) {
        case multiplication_rule::trivial: {
            result._digits.resize(_digits.size() + other._digits.size(), 0);

            for (size_t i = 0; i < _digits.size(); ++i) {
                unsigned long long carry = 0;
                for (size_t j = 0; j < other._digits.size() || carry != 0; ++j) {
                    unsigned long long product = result._digits[i + j] +
                                                 static_cast<unsigned long long>(_digits[i]) *
                                                 (j < other._digits.size() ? other._digits[j] : 0) +
                                                 carry;

                    result._digits[i + j] = static_cast<unsigned int>(product % base);
                    carry = product / base;
                }
            }

            while (result._digits.size() > 1 && result._digits.back() == 0) {
                result._digits.pop_back();
            }

            result._sign = (_sign == other._sign);
            break;
        }
        case multiplication_rule::Karatsuba: {


            if (!*this || !other || _digits[0] == 0 || other._digits[0] == 0) {
                *this = big_int(0, pp_allocator<unsigned int>());
                return *this;
            }

            if (_digits.size() <= threshold || other._digits.size() <= threshold) {
                return multiply_assign(other, multiplication_rule::trivial);
            }

            size_t n = std::max(_digits.size(), other._digits.size());
            size_t half = (n + 1) / 2;

            // Разделяем числа на младшую и старшую части
            big_int low1(std::vector<unsigned int>(
                    _digits.begin(), _digits.begin() + std::min(half, _digits.size())), true);

            big_int high1(std::vector<unsigned int>(
                    _digits.begin() + std::min(half, _digits.size()), _digits.end()), true);

            big_int low2(std::vector<unsigned int>(
                    other._digits.begin(), other._digits.begin() + std::min(half, other._digits.size())), true);

            big_int high2(std::vector<unsigned int>(
                    other._digits.begin() + std::min(half, other._digits.size()), other._digits.end()), true);

            // Рекурсивные шаги Karatsuba
            big_int z0((pp_allocator<unsigned int>()));
            z0 = low1.multiply_assign(low2, multiplication_rule::Karatsuba);

            big_int z2((pp_allocator<unsigned int>()));
            z2 = high1.multiply_assign(high2, multiplication_rule::Karatsuba);

            big_int sum1 = low1 + high1;
            big_int sum2 = low2 + high2;

            big_int z1((pp_allocator<unsigned int>()));
            z1 = sum1.multiply_assign(sum2, multiplication_rule::Karatsuba);

            // Вычисляем z1 - z2 - z0
            z1 -= z2;
            z1 -= z0;

            // Собираем финальный результат
            *this = big_int(pp_allocator<unsigned int>());
            this->plus_assign(z2, 2 * half);
            this->plus_assign(z1, half);
            this->plus_assign(z0);

            _sign = (_sign == other._sign);
            return *this;
        }

        case multiplication_rule::SchonhageStrassen:
            throw not_implemented("big_int &big_int::multiply_assign(const big_int &other, multiplication_rule rule) &", "your code should be here...");
    }

    *this = std::move(result);
    return *this;
}

big_int &big_int::divide_assign(const big_int &other, big_int::division_rule rule) &
{
    if (other._digits.empty() || (other._digits.size() == 1 && other._digits[0] == 0)) {
        throw std::invalid_argument("Division by zero is not allowed.");
    }


    big_int result((pp_allocator<unsigned int>()));

    switch (rule) {
        case division_rule::trivial: {
            big_int dividend = *this;
            big_int divisor = other;
            result = big_int(0);

            dividend._sign = true;
            divisor._sign = true;

            while (dividend >= divisor) {
                big_int temp_divisor = divisor;
                big_int quotient = 1;

                while ((temp_divisor << 1) <= dividend) {
                    temp_divisor <<= 1;
                    quotient <<= 1;
                }

                dividend -= temp_divisor;
                result += quotient;
            }

            result._sign = (_sign == other._sign);
            break;
        }
        case division_rule::Newton:
        case division_rule::BurnikelZiegler:
            throw not_implemented("big_int &big_int::divide_assign(const big_int &other, division_rule rule) &", "your code should be here...");
    }

    *this = std::move(result);
    return *this;
}

big_int &big_int::modulo_assign(const big_int &other, big_int::division_rule rule) &
{
    throw not_implemented("big_int &big_int::modulo_assign(const big_int &other, big_int::division_rule rule) &", "your code should be here...");
}

big_int operator""_bi(unsigned long long n)
{
    return n;
}


big_int big_int::operator-() const {
    big_int result = *this;
    result._sign = !_digits.empty() && !(_digits.size() == 1 && _digits[0] == 0) && !result._sign;
    return result;
}

big_int big_int::factorial(big_int n)
{
    if (n < big_int("0")){
        throw std::invalid_argument("Must be not negative number");
    }
    if (n == big_int("0") || n == big_int("1"))
    {
        return big_int("1");
    }
    return n * factorial(n-big_int("1"));
}


