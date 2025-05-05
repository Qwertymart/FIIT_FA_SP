#include "../include/fraction.h"



void fraction::optimise() {
    if (_numerator == 0_bi) {
        _denominator = 1_bi;
        return;
    }

    big_int g = gcd(::abs(_numerator), ::abs(_denominator));
    _numerator /= g;
    _denominator /= g;

    if (_denominator < 0_bi) {
        _numerator = -_numerator;
        _denominator = -_denominator;
    }
}

fraction::fraction(pp_allocator<big_int::value_type> allocator)
        : _numerator(1, allocator), _denominator(1, allocator) {}

fraction& fraction::operator+=(const fraction& other) & {
    _numerator = _numerator * other._denominator + other._numerator * _denominator;
    _denominator *= other._denominator;
    optimise();
    return *this;
}

fraction fraction::operator+(const fraction& other) const {
    fraction result = *this;
    result += other;
    return result;
}

fraction& fraction::operator-=(const fraction& other) & {
    _numerator = _numerator * other._denominator - other._numerator * _denominator;
    _denominator *= other._denominator;
    optimise();
    return *this;
}

fraction fraction::operator-(const fraction& other) const {
    fraction result = *this;
    result -= other;
    return result;
}

fraction& fraction::operator*=(const fraction& other) & {
    _numerator *= other._numerator;
    _denominator *= other._denominator;
    optimise();
    return *this;
}

fraction fraction::operator*(const fraction& other) const {
    fraction result = *this;
    result *= other;
    return result;
}

fraction& fraction::operator/=(const fraction& other) & {
    if (other._numerator == 0_bi) {
        throw std::invalid_argument("Division by zero in fraction::operator/=");
    }

    _numerator *= other._denominator;
    _denominator *= other._numerator;
    optimise();
    return *this;
}

fraction fraction::operator/(const fraction& other) const {
    fraction result = *this;
    result /= other;
    return result;
}

bool fraction::operator==(const fraction& other) const noexcept {
    return _numerator == other._numerator && _denominator == other._denominator;
}

std::partial_ordering fraction::operator<=>(const fraction& other) const noexcept {
    return (_numerator * other._denominator) <=> (other._numerator * _denominator);
}

std::ostream& operator<<(std::ostream& stream, const fraction& obj) {
    return stream << obj._numerator << "/" << obj._denominator;
}

std::istream& operator>>(std::istream& stream, fraction& obj) {
    std::string input;
    stream >> input;
    auto slash_pos = input.find('/');
    if (slash_pos == std::string::npos)
        throw std::invalid_argument("Invalid fraction format");

    obj._numerator = big_int(input.substr(0, slash_pos));
    obj._denominator = big_int(input.substr(slash_pos + 1));
    if (obj._denominator == 0_bi)
        throw std::invalid_argument("Denominator cannot be zero");
    obj.optimise();
    return stream;
}

std::string fraction::to_string() const {
    return _numerator.to_string() + "/" + _denominator.to_string();
}

fraction fraction::abs() const {
    fraction result = *this;
    if (result._numerator < 0_bi)
        result._numerator = -result._numerator;
    return result;
}

fraction fraction::pow(size_t degree) const {
    if (degree == 0)
        return fraction(1_bi, 1_bi);
    if (degree == 1)
        return *this;

    fraction result(1_bi, 1_bi);
    fraction base = *this;

    while (degree) {
        if (degree & 1) result *= base;
        base *= base;
        degree >>= 1;
    }
    return result;
}

fraction fraction::sin(const fraction& epsilon) const {
    fraction result(0_bi, 1_bi);
    fraction term = *this;
    size_t n = 0;
    int sign = 1;

    while (term.abs() > epsilon) {
        result += term * fraction(sign, 1_bi);
        ++n;
        term = this->pow(2 * n + 1) / fraction(big_int::factorial(2 * n + 1), 1_bi);
        sign *= -1;
    }
    return result;
}

fraction fraction::cos(const fraction& epsilon) const {
    fraction result(0_bi, 1_bi);
    fraction term(1_bi, 1_bi);
    size_t n = 0;
    int sign = 1;

    while (term.abs() > epsilon) {
        result += term * fraction(sign, 1_bi);
        ++n;
        term = this->pow(2 * n) / fraction(big_int::factorial(2 * n), 1_bi);
        sign *= -1;
    }
    return result;
}

fraction fraction::tg(const fraction& epsilon) const {
    return this->sin(epsilon) / this->cos(epsilon);
}

fraction fraction::ctg(const fraction& epsilon) const {
    return this->cos(epsilon) / this->sin(epsilon);
}

fraction fraction::sec(const fraction& epsilon) const {
    return fraction(1_bi, 1_bi) / this->cos(epsilon);
}

fraction fraction::cosec(const fraction& epsilon) const {
    return fraction(1_bi, 1_bi) / this->sin(epsilon);
}

fraction fraction::arcsin(const fraction& epsilon) const {
    if (*this < fraction(-1_bi, 1_bi) || *this > fraction(1_bi, 1_bi))
        throw std::domain_error("arcsin requires |x| ≤ 1");

    fraction result = *this;
    fraction x_sq = *this * *this;
    fraction term = *this;
    size_t n = 1;

    while (term.abs() > epsilon) {
        term *= x_sq;
        term *= fraction((2 * n - 1) * (2 * n - 1), (2 * n) * (2 * n + 1));
        result += term;
        ++n;
    }
    return result;
}

fraction fraction::arccos(const fraction& epsilon) const {
    return fraction(big_int("15707963267948966"), big_int("10000000000000000")) - this->arcsin(epsilon);// optional if acos defined separately
}

fraction fraction::arctg(const fraction& epsilon) const {
    fraction result = *this;
    fraction x_sq = *this * *this;
    fraction term = *this;
    size_t n = 1;
    int sign = -1;

    while (term.abs() > epsilon) {
        term *= x_sq;
        result += term / fraction(2 * n + 1, 1_bi) * fraction(sign, 1_bi);
        ++n;
        sign *= -1;
    }
    return result;
}

fraction fraction::arcctg(const fraction& epsilon) const {
    // Вычисляем арктангенс
    fraction arctg_result = this->arctg(epsilon);

    // π/2 — это фиксированное значение, которое можно записать как fraction
    const fraction pi_over_2(big_int("15707963267948966"), big_int("10000000000000000"));

    // Возвращаем разницу π/2 - arctg(x)
    return pi_over_2 - arctg_result;
}


fraction fraction::arcsec(fraction const &epsilon) const
{
    return (fraction(1_bi, 1_bi) / (*this)).arccos(epsilon);
}

fraction fraction::arccosec(const fraction& epsilon) const {
    return (fraction(1_bi, 1_bi) / (*this)).arcsin(epsilon);
}

fraction fraction::root(size_t degree, const fraction& epsilon) const {
    if (*this == fraction(0_bi, 1_bi))
        return *this;

    fraction x = *this;
    fraction guess = fraction(1_bi, 1_bi);
    fraction delta;

    do {
        fraction prev = guess;
        guess = (fraction(big_int(degree - 1), big_int(1)) * guess + x / guess.pow(degree - 1)) / fraction(big_int(degree), big_int(1));

        delta = guess - prev;
    } while (delta.abs() > epsilon);

    return guess;
}

fraction fraction::ln(fraction const &epsilon) const {
    if (*this <= fraction(0_bi, 1_bi)) {
        throw std::domain_error("ln is undefined for non-positive numbers");
    }

    if (*this <= fraction(0_bi, 1_bi))
        throw std::domain_error("ln(x): x must be > 0");

    if (*this == fraction(1_bi, 1_bi))
        return fraction(0_bi, 1_bi);

    if (*this < fraction(1_bi, 1_bi))
    {
        fraction inv = fraction(1_bi, 1_bi) / (*this);
        return fraction(-1_bi, 1_bi) * inv.ln(epsilon);
    }

    fraction x = *this;
    fraction one(1_bi, 1_bi);
    fraction two(2_bi, 1_bi);

    big_int pow_shift(0_bi);
    while (x > two) {
        x /= two;
        ++pow_shift;
    }
    while (x < fraction(1_bi, 2_bi)) {
        x *= two;
        --pow_shift;
    }

    // Используем разложение Артана
    fraction y = (x - one) / (x + one); // y = (x - 1) / (x + 1)
    fraction y2 = y * y;

    fraction term = y;
    fraction sum = term;
    big_int n = 1_bi;

    while (true) {
        term = term * y2;
        n += 2_bi;
        fraction next = term / fraction(n, 1_bi);

        if (next.abs() < epsilon) break;
        sum += next;
    }

    fraction ln2(big_int("6931471805599"), big_int("10000000000000"));

    // ln(x) ≈ 2 * sum + pow_shift * ln(2)
    return two * sum + fraction(pow_shift, 1_bi) * ln2;
}

fraction ln2_approx() {
    return fraction(
            big_int("6931471805599"),
            big_int("10000000000000")
    );
}

fraction fraction::log2(const fraction &epsilon) const {
    // fraction ln2 = fraction(2_bi, 1_bi).ln(epsilon); // ненадёжно
    fraction ln2 = ln2_approx();
    return ln(epsilon) / ln2;
}


fraction fraction::lg(fraction const &epsilon) const
{
    fraction temp = ln(epsilon) / (fraction(10_bi, 1_bi)).ln(epsilon);
    temp.optimise();
    return temp;
}

