# Text Generation

_[Game Library by Ross Smith](index.html)_

```c++
#include "rs-game/text-gen.hpp"
namespace RS::Game;
```

## Text generators

```c++
using StringFunction = std::function<std::string(const std::string&)>;
using StringList = std::vector<std::string>;
using TextList = std::vector<TextGenerator>;
using TextWeights = std::vector<std::pair<TextGenerator, double>>;
```

Aliases for convenience.

```c++
class TextGenerator {
    using result_type = std::string;
    TextGenerator();
    TextGenerator(const std::string& str);
    TextGenerator(const std::string_view& str);
    TextGenerator(const char* str);
    TextGenerator(char c);
    TextGenerator(char32_t c);
    TextGenerator(const TextGenerator& g);
    TextGenerator(TextGenerator&& g) noexcept;
    ~TextGenerator() noexcept;
    TextGenerator& operator=(const TextGenerator& g);
    TextGenerator& operator=(TextGenerator&& g) noexcept;
    std::string operator()(Sci::StdRng& rng) const;
};
```

The text generator class. A text generator constructed in the normal way
yields a fixed string, or an empty string if it was default constructed. The
functions and operators below act as generator combinators.

```c++
template <typename T> TextGenerator str(T&& t);
```

The argument can be a string or character; this calls the corresponding
`TextGenerator` constructor.

```c++
TextGenerator number(int min, int max);
```

Generates a random integer between `min` and `max` inclusive, and returns it
as a string. This will throw `std::invalid_argument` if `min>max`.

```c++
TextGenerator choose(const std::string& list);
TextGenerator choose(const StringList& list);
TextGenerator choose(const TextList& list);
TextGenerator choose(std::initializer_list<TextGenerator> list);
TextGenerator choose(const TextWeights& weights);
TextGenerator choose(std::initializer_list<std::pair<TextGenerator, double>> weights);
```

These create a generator that calls one of a set of generators, chosen at
random. The first four versions choose any of their generators with equal
probability (the first version chooses one of the Unicode characters in the
string). The last two choose with probabilities in proportion to the
specified weights (weights need not add up to 1).

All of these will throw `std::invalid_argument` if the argument container is
empty. The first version will throw if the string is not valid UTF-8. The
weighted versions will throw if any weight is negative, or if all weights are
zero.

```c++
TextGenerator operator+(const TextGenerator& a, const TextGenerator& b);
```

Concatenates the output of two generators.

```c++
TextGenerator operator|(const TextGenerator& a, const TextGenerator& b);
```

Returns the result of either generator at random. This operator can be chained
to create multiple choices with equal probability, as in the unweighted
versions of `choose()`; for example, `A|B|C|D` will call one of the generators
with probability 1/4, equivalent to `choose(A,B,C,D)`.

```c++
TextGenerator operator*(const TextGenerator& g, int n);
```

`G*n` calls the generator `n` times and concatenates the results; `G*m*n`
generates a random number from `m` to `n` and concatenates that many calls.
This will throw `std::invalid_argument` if `m>n`.

```c++
TextGenerator operator%(const TextGenerator& g, double p);
```

Calls the generator with probability `p`, otherwise returns an empty string.
This will throw `std::invalid_argument` if `p<0` or `p>1`.

```c++
TextGenerator operator>>(const TextGenerator& g, StringFunction f);
```

Calls the function on the output of the generator and returns the result.
