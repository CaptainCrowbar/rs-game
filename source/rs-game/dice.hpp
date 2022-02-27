#pragma once

#include "rs-format/string.hpp"
#include "rs-sci/rational.hpp"
#include <map>
#include <memory>
#include <mutex>
#include <ostream>
#include <random>
#include <string>
#include <vector>

namespace RS::Game {

    class Dice {

    public:

        using result_type = Sci::Rational;

        Dice() = default;
        explicit Dice(int n) { insert(n, 6, 1); modified(); }
        Dice(int n, int faces, const Sci::Rational& factor = 1) { insert(n, faces, factor); modified(); }
        explicit Dice(const std::string& str);

        template <typename RNG> Sci::Rational operator()(RNG& rng);

        Dice operator+() const { return *this; }
        Dice operator-() const;
        Dice& operator+=(const Dice& b);
        Dice& operator+=(const Sci::Rational& b);
        Dice& operator+=(int b);
        Dice& operator-=(const Dice& b);
        Dice& operator-=(const Sci::Rational& b);
        Dice& operator-=(int b);
        Dice& operator*=(const Sci::Rational& b);
        Dice& operator*=(int b) { return *this *= Sci::Rational(b); }
        Dice& operator/=(const Sci::Rational& b) { return *this *= b.reciprocal(); }
        Dice& operator/=(int b) { return *this *= Sci::Rational(int(1), b); }

        Sci::Rational mean() const noexcept;
        Sci::Rational variance() const noexcept;
        double sd() const noexcept;
        Sci::Rational min() const noexcept { return min_; }
        Sci::Rational max() const noexcept { return max_; }
        Sci::Rational pdf(const Sci::Rational& x) const;
        Sci::Rational cdf(const Sci::Rational& x) const;
        Sci::Rational ccdf(const Sci::Rational& x) const;
        Sci::Rational interval(const Sci::Rational& x, const Sci::Rational& y) const;
        std::string str() const;

    private:

        using distribution_type = std::uniform_int_distribution<int>;
        using pdf_table = std::map<Sci::Rational, Sci::Rational>;

        struct dice_group {
            int number;
            distribution_type each;
            Sci::Rational factor;
        };

        struct probabilites {
            Sci::Rational pdf;
            Sci::Rational cdf;
            Sci::Rational ccdf;
        };

        using probability_table = std::map<Sci::Rational, probabilites>;

        struct table_info {
            probability_table table;
            std::mutex mutex;
        };

        std::vector<dice_group> groups_;
        Sci::Rational add_;
        Sci::Rational min_;
        Sci::Rational max_;
        std::shared_ptr<table_info> info_;

        bool check_table() const;
        void insert(int n, int faces, const Sci::Rational& factor);
        void modified();

        static pdf_table make_table(const dice_group& group);

    };

        template <typename RNG>
        Sci::Rational Dice::operator()(RNG& rng) {
            Sci::Rational sum = add_;
            for (auto& g: groups_) {
                int roll = 0;
                for (int i = 0; i < g.number; ++i)
                    roll += g.each(rng);
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
