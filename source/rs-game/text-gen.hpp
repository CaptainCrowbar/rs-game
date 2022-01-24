#pragma once

#include "rs-sci/random.hpp"
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <utility>
#include <vector>

namespace RS::Game {

    class TextGenerator;
    using StringFunction = std::function<std::string(const std::string&)>;
    using StringList = std::vector<std::string>;
    using TextList = std::vector<TextGenerator>;
    using TextWeights = std::vector<std::pair<TextGenerator, double>>;

    namespace Detail {

        class TextBase;
        using SharedBase = std::shared_ptr <TextBase>;
        using BaseList = std::vector<SharedBase>;
        using BaseWeights = std::vector<std::pair<SharedBase, double>>;

        template <typename T, typename... Args> TextGenerator base2gen(Args&&... args);
        SharedBase gen2base(const TextGenerator& tg);

    }

    class TextGenerator {
    public:
        using result_type = std::string;
        TextGenerator() = default;
        TextGenerator(const std::string& str);
        TextGenerator(const std::string_view& str): TextGenerator(std::string(str)) {}
        TextGenerator(const char* str): TextGenerator(std::string(str)) {}
        TextGenerator(char c): TextGenerator(std::string{c}) {}
        TextGenerator(char32_t c);
        std::string operator()(Sci::StdRng& rng) const;
    private:
        template <typename T, typename... Args> friend TextGenerator Detail::base2gen(Args&&... args);
        friend Detail::SharedBase Detail::gen2base(const TextGenerator& tg);
        Detail::SharedBase base_;
    };

    namespace Detail {

        class TextBase {
        public:
            using result_type = std::string;
            virtual ~TextBase() = default;
            virtual std::string gen(Sci::StdRng& rng) const = 0;
            std::string operator()(Sci::StdRng& rng) const { return gen(rng); }
        };

        template <typename T, typename... Args>
        TextGenerator base2gen(Args&&... args) {
            TextGenerator tg;
            tg.base_ = std::make_shared<T>(std::forward<Args>(args)...);
            return tg;
        }

        class FixedText:
        public TextBase {
        public:
            explicit FixedText(const std::string& str): str_(str) {}
            std::string gen(Sci::StdRng& /*rng*/) const override { return str_; }
        private:
            std::string str_;
        };

        class NumberText:
        public TextBase {
        public:
            NumberText(int min, int max): random_int_(min, max) {}
            std::string gen(Sci::StdRng& rng) const override;
        private:
            Sci::UniformInteger<int> random_int_;
        };

        class SelectText:
        public TextBase {
        public:
            explicit SelectText(const BaseList& list): list_(list), choice_(list) {}
            std::string gen(Sci::StdRng& rng) const override;
            const BaseList& list() const noexcept { return list_; }
        private:
            BaseList list_;
            Sci::RandomChoice<SharedBase> choice_;
        };

        class WeightedText:
        public TextBase {
        public:
            explicit WeightedText(const BaseWeights& weights);
            std::string gen(Sci::StdRng& rng) const override;
        private:
            Sci::WeightedChoice<SharedBase> choice_;
        };

        class SequenceText:
        public TextBase {
        public:
            SequenceText(SharedBase left, SharedBase right): left_(left), right_(right) {}
            std::string gen(Sci::StdRng& rng) const override;
        private:
            SharedBase left_;
            SharedBase right_;
        };

        class OptionalText:
        public TextBase {
        public:
            OptionalText(SharedBase base, double p): base_(base), option_(p) {}
            std::string gen(Sci::StdRng& rng) const override;
        private:
            SharedBase base_;
            Sci::BernoulliDistribution option_;
        };

        class RepeatText:
        public TextBase {
        public:
            RepeatText(SharedBase base, int n): base_(base), random_int_(n, n) {}
            RepeatText(SharedBase base, int m, int n): base_(base), random_int_(m, n) {}
            std::string gen(Sci::StdRng& rng) const override;
            SharedBase base() const noexcept { return base_; }
            int min() const noexcept { return random_int_.min(); }
        private:
            SharedBase base_;
            Sci::UniformInteger<int> random_int_;
        };

        class TransformText:
        public TextBase {
        public:
            TransformText(SharedBase base, StringFunction f): base_(base), fun_(f) {}
            std::string gen(Sci::StdRng& rng) const override;
        private:
            SharedBase base_;
            StringFunction fun_;
        };

    }

    // A+B    A followed by B
    // A|B    A or B with equal probability
    // A*n    A repeated n times
    // A*m*n  A repeated m-n times
    // A%p    A with probability p
    // A>>f   Return f(A)

    TextGenerator operator+(const TextGenerator& a, const TextGenerator& b);
    TextGenerator operator|(const TextGenerator& a, const TextGenerator& b);
    TextGenerator operator*(const TextGenerator& tg, int n);
    TextGenerator operator%(const TextGenerator& tg, double p);
    TextGenerator operator>>(const TextGenerator& tg, StringFunction f);

    TextGenerator str(const std::string& s);
    TextGenerator number(int min, int max);
    TextGenerator choose(const StringList& list);
    TextGenerator choose(const TextList& list);
    TextGenerator choose(std::initializer_list<TextGenerator> list);
    TextGenerator choose(const TextWeights& weights);
    TextGenerator choose(std::initializer_list<std::pair<TextGenerator, double>> list);

}
