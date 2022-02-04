# Text Generation

_[Game Library by Ross Smith](index.html)_

```c++
#include "rs-game/text-gen.hpp"
namespace RS::Game;
```

## Contents

* TOC
{:toc}

## Supporting types

```c++
using StringFunction = std::function<std::string(const std::string&)>;
using StringList = std::vector<std::string>;
using TextList = std::vector<TextGen>;
using TextWeights = std::vector<std::pair<TextGen, double>>;
```

Aliases for convenience.

## TextGen class

```c++
class TextGen;
```

The text generator class.

```c++
using result_type = std::string;
```

Return type.

```c++
TextGen();
```

A default constructed generator will always return an empty string.

```c++
TextGen(const std::string& str);
TextGen(const std::string_view& str);
TextGen(const char* str);
TextGen(char c);
TextGen(char32_t c);
```

All of these construct a generator that always returns the same string.

```c++
TextGen(const TextGen& g);
TextGen(TextGen&& g) noexcept;
~TextGen() noexcept;
TextGen& operator=(const TextGen& g);
TextGen& operator=(TextGen&& g) noexcept;
```

Other life cycle functions.

```c++
std::string operator()(Sci::StdRng& rng) const;
```

Generates a string.

```c++
staticTextGen TextGen::number(int min, int max);
```

Generates a random integer between `min` and `max` inclusive, and returns it
as a decimal string. This will throw `std::invalid_argument` if `min>max`.

```c++
staticTextGen TextGen::choice(const std::string& list);
staticTextGen TextGen::choice(const StringList& list);
staticTextGen TextGen::choice(const TextList& list);
staticTextGen TextGen::choice(std::initializer_list<TextGen> list);
staticTextGen TextGen::choice(const TextWeights& weights);
staticTextGen TextGen::choice(std::initializer_list<
    std::pair<TextGen, double>> weights);
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

## Generator combinators

```c++
TextGen operator+(const TextGen& a, const TextGen& b);
TextGen& operator+=(TextGen& a, const TextGen& b);
```

Concatenates the output of two generators.

```c++
TextGen operator|(const TextGen& a, const TextGen& b);
TextGen& operator|=(TextGen& a, const TextGen& b);
```

Returns the result of either generator at random. This operator can be chained
to create multiple choices with equal probability, as in the unweighted
versions of `choice()`; for example, `A|B|C|D` will call one of the generators
with probability 1/4, equivalent to `choice(A,B,C,D)`.

```c++
TextGen operator*(const TextGen& g, int n);
TextGen& operator*=(TextGen& g, int n);
```

`G*n` calls the generator `n` times and concatenates the results; `G*m*n`
generates a random number from `m` to `n` and concatenates that many calls.
This will throw `std::invalid_argument` if `m>n`.

```c++
TextGen operator%(const TextGen& g, double p);
TextGen& operator%=(TextGen& g, double p);
```

Calls the generator with probability `p`, otherwise returns an empty string.
This will throw `std::invalid_argument` if `p<0` or `p>1`.

```c++
TextGen operator>>(const TextGen& g, StringFunction f);
TextGen& operator>>=(TextGen& g, StringFunction f);
```

Calls the function on the output of the generator and returns the result.

## Generator literals

```c++
namespace Literals {
    TextGen operator""_tg(const char* ptr, size_t len);
    TextGen operator""_tg(unsigned long long n);
}
```

The string-based literal splits the string into words delimited by whitespace,
and constructs a generator that will return one of the strings at random. The
integer-based literal is equivalent to `TextGen::number(1,n)`.
