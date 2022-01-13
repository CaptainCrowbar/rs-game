#pragma once

#include "rs-format/string.hpp"
#include "rs-sci/rational.hpp"
#include <ostream>
#include <random>
#include <string>
#include <vector>

namespace RS::Game {

    class Dice {

    public:

        using result_type = Sci::Rational;

        Dice() = default;
        explicit Dice(int n) { insert(n, 6, 1); }
        Dice(int n, int faces, const Sci::Rational& factor = 1) { insert(n, faces, factor); }
        explicit Dice(const std::string& str);

        template <typename RNG> Sci::Rational operator()(RNG& rng);

        Dice operator+() const { return *this; }
        Dice operator-() const;
        Dice& operator+=(const Dice& b);
        Dice& operator+=(const Sci::Rational& b) { modifier_ += b; return *this; }
        Dice& operator+=(int b) { modifier_ += b; return *this; }
        Dice& operator-=(const Dice& b);
        Dice& operator-=(const Sci::Rational& b) { modifier_ -= b; return *this; }
        Dice& operator-=(int b) { modifier_ -= b; return *this; }
        Dice& operator*=(const Sci::Rational& b);
        Dice& operator*=(int b) { return *this *= Sci::Rational(b); }
        Dice& operator/=(const Sci::Rational& b) { return *this *= b.reciprocal(); }
        Dice& operator/=(int b) { return *this *= Sci::Rational(int(1), b); }

        Sci::Rational mean() const noexcept;
        Sci::Rational variance() const noexcept;
        double sd() const noexcept;
        Sci::Rational min() const noexcept;
        Sci::Rational max() const noexcept;
        std::string str() const;

    private:

        using distribution_type = std::uniform_int_distribution<int>;

        struct dice_group {
            distribution_type one_dice;
            int n_dice;
            Sci::Rational factor;
        };

        std::vector<dice_group> groups_;
        Sci::Rational modifier_;

        void insert(int n, int faces, const Sci::Rational& factor);

    };

        template <typename RNG>
        Sci::Rational Dice::operator()(RNG& rng) {
            Sci::Rational sum = modifier_;
            for (auto& g: groups_) {
                int roll = 0;
                for (int i = 0; i < g.n_dice; ++i)
                    roll += g.one_dice(rng);
                sum += roll * g.factor;
            }
            return sum;
        }

    inline Dice operator+(const Dice& a, const Dice& b) { auto d = a; d += b; return d; }
    inline Dice operator+(const Dice& a, const Sci::Rational& b) { auto d = a; d += b; return d; }
    inline Dice operator+(const Dice& a, int b) { auto d = a; d += b; return d; }
    inline Dice operator+(const Sci::Rational& a, const Dice& b) { auto d = b; d += a; return d; }
    inline Dice operator+(int a, const Dice& b) { auto d = b; d += a; return d; }
    inline Dice operator-(const Dice& a, const Dice& b) { auto d = a; d -= b; return d; }
    inline Dice operator-(const Dice& a, const Sci::Rational& b) { auto d = a; d -= b; return d; }
    inline Dice operator-(const Dice& a, int b) { auto d = a; d -= b; return d; }
    inline Dice operator-(const Sci::Rational& a, const Dice& b) { auto d = - b; d += a; return d; }
    inline Dice operator-(int a, const Dice& b) { auto d = - b; d += a; return d; }
    inline Dice operator*(const Dice& a, const Sci::Rational& b) { auto d = a; d *= b; return d; }
    inline Dice operator*(const Dice& a, int b) { auto d = a; d *= b; return d; }
    inline Dice operator*(const Sci::Rational& a, const Dice& b) { auto d = b; d *= a; return d; }
    inline Dice operator*(int a, const Dice& b) { auto d = b; d *= a; return d; }
    inline Dice operator/(const Dice& a, const Sci::Rational& b) { auto d = a; d /= b; return d; }
    inline Dice operator/(const Dice& a, int b) { auto d = a; d /= b; return d; }
    inline std::ostream& operator<<(std::ostream& out, const Dice& d) { return out << d.str(); }

    namespace Literals {

        inline Dice operator""_d3(unsigned long long n) { return Dice(int(n), 3); }
        inline Dice operator""_d4(unsigned long long n) { return Dice(int(n), 4); }
        inline Dice operator""_d6(unsigned long long n) { return Dice(int(n), 6); }
        inline Dice operator""_d8(unsigned long long n) { return Dice(int(n), 8); }
        inline Dice operator""_d10(unsigned long long n) { return Dice(int(n), 10); }
        inline Dice operator""_d12(unsigned long long n) { return Dice(int(n), 12); }
        inline Dice operator""_d20(unsigned long long n) { return Dice(int(n), 20); }
        inline Dice operator""_d30(unsigned long long n) { return Dice(int(n), 30); }
        inline Dice operator""_d100(unsigned long long n) { return Dice(int(n), 100); }
        inline Dice operator""_d1000(unsigned long long n) { return Dice(int(n), 1000); }
        inline Dice operator""_dice(const char* p, size_t n) { return Dice(std::string(p, n)); }

    }

}
