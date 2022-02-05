#include "rs-game/dice.hpp"
#include "rs-regex/regex.hpp"
#include "rs-sci/algorithm.hpp"
#include "rs-tl/algorithm.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <utility>

using namespace RS::RE;
using namespace RS::Sci;

namespace RS::Game {

    Dice::Dice(const std::string& str) {

        static const auto parse_integer = [] (const std::string& str, int def) noexcept {
            return str.empty() ? def : Format::to_integer<int>(str);
        };

        static const Regex pattern(R"(
            ( [+-] )                # [1] sign
            (?:
                (?: (\d+) [*x] ) ?  # [2] left multiplier
                (\d*)               # [3] number of dice
                d (\d*)             # [4] number of faces
                (?: [*x] (\d+) ) ?  # [5] right multiplier
            |
                (\d+)               # [6] fixed modifier
            )
            (?: / (\d+) ) ?         # [7] divisor
        )", Regex::anchor | Regex::extended | Regex::icase | Regex::optimize);

        std::string text(str);
        TL::remove_in_if(text, Format::ascii_isspace);

        if (text.empty())
            return;

        if (text[0] != '+' && text[0] != '-')
            text.insert(0, 1, '+');

        size_t pos = 0;

        while (pos < text.size()) {

            auto match = pattern(text, pos);
            auto sign = *match.begin() == '-' ? -1 : 1;
            auto divisor = parse_integer(std::string(match[7]), 1);

            if (match.matched(6)) {

                auto factor = parse_integer(std::string(match[6]), 1);
                modifier_ += Rational(sign * factor, divisor);

            } else {

                auto factor1 = parse_integer(std::string(match[2]), 1);
                auto n_dice = parse_integer(std::string(match[3]), 1);
                auto n_faces = parse_integer(std::string(match[4]), 6);
                auto factor2 = parse_integer(std::string(match[5]), 1);
                insert(n_dice, n_faces, Rational(sign * factor1 * factor2, divisor));

            }

            pos = match.endpos();

        }

        updated();

    }

    Dice Dice::operator-() const {
        Dice d = *this;
        for (auto& g: d.groups_)
            g.factor = - g.factor;
        d.modifier_ = - d.modifier_;
        d.updated();
        return d;
    }

    Dice& Dice::operator+=(const Dice& rhs) {
        Dice d = *this;
        for (auto& g: rhs.groups_)
            d.insert(g.number, g.each.b(), g.factor);
        d.modifier_ += rhs.modifier_;
        d.updated();
        *this = std::move(d);
        return *this;
    }

    Dice& Dice::operator+=(const Rational& b) {
        modifier_ += b;
        updated();
        return *this;
    }

    Dice& Dice::operator+=(int b) {
        modifier_ += b;
        updated();
        return *this;
    }

    Dice& Dice::operator-=(const Dice& rhs) {
        Dice d = *this;
        for (auto& g: rhs.groups_)
            d.insert(g.number, g.each.b(), - g.factor);
        d.modifier_ -= rhs.modifier_;
        d.updated();
        *this = std::move(d);
        return *this;
    }

    Dice& Dice::operator-=(const Rational& b) {
        modifier_ -= b;
        updated();
        return *this;
    }

    Dice& Dice::operator-=(int b) {
        modifier_ -= b;
        updated();
        return *this;
    }

    Dice& Dice::operator*=(const Rational& rhs) {
        if (rhs) {
            for (auto& g: groups_)
                g.factor *= rhs;
            modifier_ *= rhs;
        } else {
            groups_.clear();
            modifier_ = 0;
        }
        updated();
        return *this;
    }

    Rational Dice::mean() const noexcept {
        Rational sum = modifier_;
        for (auto& g: groups_)
            sum += Rational(g.number * (g.each.b() + 1)) * g.factor / Rational(2);
        return sum;
    }

    Rational Dice::variance() const noexcept {
        Rational sum;
        for (auto& g: groups_)
            sum += Rational(g.number * (g.each.b() * g.each.b() - 1)) * g.factor * g.factor / Rational(12);
        return sum;
    }

    double Dice::sd() const noexcept {
        using std::sqrt;
        return sqrt(double(variance()));
    }

    Rational Dice::pdf(const Rational& x) {
        update_table();
        auto it = table_->find(x);
        if (it == table_->end())
            return 0;
        else
            return it->second.pdf;
    }

    Rational Dice::cdf(const Rational& x) {
        update_table();
        auto it = table_->lower_bound(x);
        if (it == table_->end())
            return 1;
        else if (it->first == x)
            return it->second.cdf;
        else if (it == table_->begin())
            return 0;
        else
            return std::prev(it)->second.cdf;
    }

    Rational Dice::ccdf(const Rational& x) {
        update_table();
        auto it = table_->lower_bound(x);
        if (it == table_->end())
            return 0;
        else
            return it->second.ccdf;
    }

    std::string Dice::str() const {

        std::string text;

        for (auto& g: groups_) {
            text += g.factor.sign() == -1 ? '-' : '+';
            if (g.number > 1)
                text += std::to_string(g.number);
            text += 'd' + std::to_string(g.each.b());
            auto n = std::abs(g.factor.num());
            if (n > 1)
                text += '*' + std::to_string(n);
            auto d = g.factor.den();
            if (d > 1)
                text += '/' + std::to_string(d);
        }

        if (modifier_ > 0)
            text += '+';
        if (modifier_)
            text += modifier_.str();

        if (text[0] == '+')
            text.erase(0, 1);
        if (text.empty())
            text = "0";

        return text;

    }

    void Dice::insert(int n, int faces, const Rational& factor) {

        static const auto match_terms = [] (const dice_group& g1, const dice_group& g2) noexcept {
            return g1.each.b() == g2.each.b() && g1.factor == g2.factor;
        };

        static const auto sort_terms = [] (const dice_group& g1, const dice_group& g2) noexcept {
            return g1.each.b() == g2.each.b() ? g1.factor < g2.factor : g1.each.b() > g2.each.b();
        };

        if (n < 0 || faces < 0)
            throw std::invalid_argument("Invalid dice");

        if (n > 0 && faces > 0 && factor != 0) {
            dice_group g;
            g.number = n;
            g.each = distribution_type(1, faces);
            g.factor = factor;
            auto it = std::lower_bound(groups_.begin(), groups_.end(), g, sort_terms);
            if (it != groups_.end() && match_terms(*it, g))
                it->number += g.number;
            else
                groups_.insert(it, g);
        }

    }

    void Dice::updated() noexcept {

        min_ = max_ = modifier_;

        for (auto& g: groups_) {
            if (g.factor > 0) {
                min_ += Rational(g.number) * g.factor;
                max_ += Rational(g.number * g.each.b()) * g.factor;
            } else {
                min_ += Rational(g.number * g.each.b()) * g.factor;
                max_ += Rational(g.number) * g.factor;
            }
        }

        table_.reset();

    }

    void Dice::update_table() {

        if (table_)
            return;

        table_ = std::make_shared<probability_table>();
        int n = int(groups_.size());
        std::vector<pdf_table> subtables(n);
        std::transform(groups_.begin(), groups_.end(), subtables.begin(), make_table);
        std::vector<pdf_table::const_iterator> iterators(n);
        std::transform(subtables.begin(), subtables.end(), iterators.begin(), [] (auto& sub) { return sub.begin(); });

        for (;;) {

            auto x = modifier_;
            Rational p = 1;

            for (auto i: iterators) {
                x += i->first;
                p *= i->second;
            }

            (*table_)[x].pdf += p;

            int i = n - 1;
            for (; i >= 0; --i) {
                ++iterators[i];
                if (iterators[i] != subtables[i].end())
                    break;
                iterators[i] = subtables[i].begin();
            }
            if (i == -1)
                break;

        }

        Rational cdf = 0;
        auto j = table_->end();

        for (auto& [x,ps]: *table_)
            (--j)->second.ccdf = ps.cdf = cdf = cdf + ps.pdf;

        table_->begin()->second.ccdf = std::prev(table_->end())->second.cdf = 1;

    }

    Dice::pdf_table Dice::make_table(const dice_group& group) {

        pdf_table table;
        int n = group.number;
        int f = group.each.max();
        int den = integer_power(f, n);
        int max = f * n;
        double b = n - 1;

        for (int i = n; i <= max; ++i) {

            int num = 0;
            int sign = 1;
            double a = i - 1;

            for (int j = 0; j < n; ++j, a -= f, sign = - sign)
                num += sign * int(std::lround(binomial(a, b) * binomial(double(n), double(j))));

            Rational x = i * group.factor;
            Rational p(num, den);
            table.insert({x, p});

        }

        return table;

    }

}
