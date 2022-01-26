#include "rs-game/text-gen.hpp"
#include "rs-tl/iterator.hpp"
#include "rs-format/unicode.hpp"
#include <algorithm>
#include <stdexcept>

using namespace RS::Format;
using namespace RS::Sci;
using namespace RS::TL;

namespace RS::Game {

    namespace Detail {

        // Functions

        SharedBase gen2base(const TextGenerator& g) {
            return g.base_;
        }

        // NumberText class

        NumberText::NumberText(int min, int max):
        random_int_(min, max) {
            if (min > max)
                throw std::invalid_argument("Invalid range for number generator");
        }

        std::string NumberText::gen(StdRng& rng) const {
            return std::to_string(random_int_(rng));
        }

        // SelectText class

        SelectText::SelectText(const BaseList& list):
        list_(list), choice_(list) {
            if (list.empty())
                throw std::invalid_argument("Empty list in text selection generator");
        }

        std::string SelectText::gen(StdRng& rng) const {
            auto child = choice_(rng);
            return child->gen(rng);
        }

        // WeightedText class

        WeightedText::WeightedText(const BaseWeights& weights) {
            if (weights.empty())
                throw std::invalid_argument("Empty list in weighted text selection generator");
            double total = 0;
            for (auto& [base,weight]: weights) {
                if (weight < 0)
                    throw std::invalid_argument("Invalid weight in weighted text selection generator");
                choice_.add(weight, base);
                total += weight;
            }
            if (total == 0)
                throw std::invalid_argument("Invalid weights in weighted text selection generator");
        }

        std::string WeightedText::gen(StdRng& rng) const {
            auto child = choice_(rng);
            return child->gen(rng);
        }

        // SequenceText class

        std::string SequenceText::gen(StdRng& rng) const {
            auto left = left_->gen(rng);
            auto right = right_->gen(rng);
            return left + right;
        }

        // OptionalText class

        OptionalText::OptionalText(SharedBase base, double p):
        base_(base), option_(p) {
            if (p < 0 || p > 1)
                throw std::invalid_argument("Invalid probability in optional text generator");
        }

        std::string OptionalText::gen(StdRng& rng) const {
            if (option_(rng))
                return base_->gen(rng);
            else
                return {};
        }

        // RepeatText class

        RepeatText::RepeatText(SharedBase base, int m, int n):
        base_(base), random_int_(m, n) {
            if (m > n)
                throw std::invalid_argument("Invalid range for repeat generator");
        }

        std::string RepeatText::gen(StdRng& rng) const {
            int n = random_int_(rng);
            std::string str;
            for (int i = 0; i < n; ++i)
                str += base_->gen(rng);
            return str;
        }

        // TransformText class

        std::string TransformText::gen(StdRng& rng) const {
            auto str = base_->gen(rng);
            return fun_(str);
        }

    }

    // TextGenerator class

    TextGenerator::TextGenerator(const std::string& str):
    base_(std::make_shared<Detail::FixedText>(str)) {}

    TextGenerator::TextGenerator(char32_t c):
    base_(std::make_shared<Detail::FixedText>(encode_utf8_string({c}))) {}

    std::string TextGenerator::operator()(StdRng& rng) const {
        if (base_)
            return base_->gen(rng);
        else
            return {};
    }

    // Generator functions

    TextGenerator operator+(const TextGenerator& a, const TextGenerator& b) {
        using namespace Detail;
        return base2gen<SequenceText>(gen2base(a), gen2base(b));
    }

    TextGenerator& operator+=(TextGenerator& a, const TextGenerator& b) {
        auto t = a + b;
        a = std::move(t);
        return a;
    }

    TextGenerator operator|(const TextGenerator& a, const TextGenerator& b) {
        using namespace Detail;
        auto base_a = gen2base(a);
        auto base_b = gen2base(b);
        auto select_a = std::dynamic_pointer_cast<SelectText>(base_a);
        auto select_b = std::dynamic_pointer_cast<SelectText>(base_b);
        BaseList list;
        if (select_a)
            list = select_a->list();
        else
            list.push_back(base_a);
        if (select_b) {
            auto& list_b = select_b->list();
            std::copy(list_b.begin(), list_b.end(), append(list));
        } else {
            list.push_back(base_b);
        }
        return base2gen<SelectText>(list);
    }

    TextGenerator& operator|=(TextGenerator& a, const TextGenerator& b) {
        auto t = a | b;
        a = std::move(t);
        return a;
    }

    TextGenerator operator*(const TextGenerator& g, int n) {
        using namespace Detail;
        auto base = gen2base(g);
        auto base_rep = std::dynamic_pointer_cast<RepeatText>(base);
        if (base_rep)
            return base2gen<RepeatText>(base_rep->base(), base_rep->min(), n);
        else
            return base2gen<RepeatText>(base, n);
    }

    TextGenerator& operator*=(TextGenerator& g, int n) {
        auto t = g * n;
        g = std::move(t);
        return g;
    }

    TextGenerator operator%(const TextGenerator& g, double p) {
        using namespace Detail;
        return base2gen<OptionalText>(gen2base(g), p);
    }

    TextGenerator& operator%=(TextGenerator& g, double p) {
        auto t = g % p;
        g = std::move(t);
        return g;
    }

    TextGenerator operator>>(const TextGenerator& g, StringFunction f) {
        using namespace Detail;
        return base2gen<TransformText>(gen2base(g), f);
    }

    TextGenerator& operator>>=(TextGenerator& g, StringFunction f) {
        auto t = g >> f;
        g = std::move(t);
        return g;
    }

    TextGenerator number(int min, int max) {
        using namespace Detail;
        return base2gen<NumberText>(min, max);
    }

    TextGenerator choose(const std::string& list) {
        using namespace Detail;
        auto utf32 = decode_string(list);
        BaseList base_list;
        std::transform(utf32.begin(), utf32.end(), append(base_list),
            [] (char32_t c) { return gen2base(encode_utf8_string({c})); });
        return base2gen<SelectText>(base_list);
    }

    TextGenerator choose(const StringList& list) {
        using namespace Detail;
        BaseList base_list;
        std::transform(list.begin(), list.end(), append(base_list),
            [] (auto& s) { return std::make_shared<FixedText>(s); });
        return base2gen<SelectText>(base_list);
    }

    TextGenerator choose(const TextList& list) {
        using namespace Detail;
        BaseList base_list;
        std::transform(list.begin(), list.end(), append(base_list), gen2base);
        return base2gen<SelectText>(base_list);
    }

    TextGenerator choose(std::initializer_list<TextGenerator> list) {
        return choose(TextList(list));
    }

    TextGenerator choose(const TextWeights& weights) {
        using namespace Detail;
        BaseWeights base_weights;
        std::transform(weights.begin(), weights.end(), append(base_weights),
            [] (auto& pair) { return std::make_pair(gen2base(pair.first), pair.second); });
        return base2gen<WeightedText>(base_weights);
    }

    TextGenerator choose(std::initializer_list<std::pair<TextGenerator, double>> weights) {
        return choose(TextWeights(weights));
    }

}
