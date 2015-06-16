Algorithm
=========

A collection of C++ classes and functions designed to be used on ranges of elements (WIP).

### Classes

- [`takram::algorithm::LeafIterator`](src/takram/algorithm/leaf_iterator.h)

### Examples

#### LeafIterator

A leaf iterator traverses all the leafs in a container that has a tree-like structure. Empty sub-containers which have no leaves will be skipped during iteration.

```cpp
using C = std::vector<int>;
using B = std::vector<C>;
using A = std::vector<B>;
using Iterator = LeafIterator<int, A::iterator, B::iterator, C::iterator>;

A a{{{}}, {{0, 1}, {2, 3}}, {{}}, {{}, {4, 5}, {}}, {{}}};
auto itr = Iterator(a.begin(), a.end());
const auto end = Iterator(a.end(), a.end());
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
