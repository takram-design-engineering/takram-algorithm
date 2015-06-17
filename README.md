Algorithm
=========

A collection of C++ classes and functions designed to be used on ranges of elements (WIP).

### Classes

- [`takram::algorithm::TupleIteratorIterator`](src/takram/algorithm/tuple_iterator_iterator.h)
- [`takram::algorithm::LeafIteratorIterator`](src/takram/algorithm/leaf_iterator_iterator.h)

## Examples

### TupleIteratorIterator

[TupleIteratorIterator](src/takram/algorithm/tuple_iterator_iterator.h) is a forward iterator that increments all the internal iterators passed to its constructor. The value type of TupleIteratorIterator is a specialization of std::tuple that holds references to the values of the internal iterators. Two TupleIteratorIterators are considered equal when both share one of the internal iterators, so that the iteration stops at the shortest distance among the containers.

```cpp
#include <iostream>
#include <iterator>
#include <numeric>
#include <vector>

#include "takram/algorithm/tuple_iterator_iterator.h"

std::vector<int> a(4);
std::vector<float> b(5);
std::vector<double> c(6);
std::iota(a.begin(), a.end(), 0);
std::iota(b.begin(), b.end(), 0);
std::iota(c.begin(), c.end(), 0);

using Iterator = takram::TupleIteratorIterator<
    decltype(a)::iterator, decltype(b)::iterator, decltype(c)::iterator>;
auto itr = Iterator(std::begin(a), std::begin(b), std::begin(c));
const auto end = Iterator(std::end(a), std::end(b), std::end(c));
for (; itr != end; ++itr) {
  auto& a = std::get<0>(*itr);
  auto& b = std::get<1>(*itr);
  auto& c = std::get<2>(*itr);
  std::cout << a << " " << b << " " << c << std::endl;
}
```

This code will output:

```
0 0 0
1 1 1
2 2 2
3 3 3
```

### LeafIteratorIterator

A [LeafIteratorIterator](src/takram/algorithm/leaf_iterator_iterator.h) traverses all the leafs in a container that has a tree-like structure.

```cpp
#include <iostream>
#include <iterator>
#include <vector>

#include "takram/algorithm/leaf_iterator_iterator.h"

using C = std::vector<int>;
using B = std::vector<C>;
using A = std::vector<B>;
using Iterator = takram::LeafIteratorIterator<A::iterator, B::iterator, C::iterator>;

A a{{{}}, {{0, 1}, {2, 3}}, {{}}, {{}, {4, 5}, {}}, {{}}};
auto itr = Iterator(std::begin(a), std::end(a));
const auto end = Iterator(std::end(a), std::end(a));
for (; itr != end; ++itr) {
  std::cout << *itr << " ";
}
```

This code will output:

```
0 1 2 3 4 5
```

## Setup Guide

Run "setup.sh" inside "scripts" directory to initialize submodules and build dependant libraries.

### Submodules

- [Google Testing Framework](https://chromium.googlesource.com/external/googletest)

## License

MIT License

Copyright (C) 2015 takram design engineering

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
