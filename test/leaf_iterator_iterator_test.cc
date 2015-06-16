//
//  leaf_iterator_iterator_test.cc
//
//  MIT License
//
//  Copyright (C) 2015 Shota Matsuda
//
//  Permission is hereby granted, free of charge, to any person obtaining a
//  copy of this software and associated documentation files (the "Software"),
//  to deal in the Software without restriction, including without limitation
//  the rights to use, copy, modify, merge, publish, distribute, sublicense,
//  and/or sell copies of the Software, and to permit persons to whom the
//  Software is furnished to do so, subject to the following conditions:
//
//  The above copyright notice and this permission notice shall be included in
//  all copies or substantial portions of the Software.
//
//  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
//  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
//  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
//  THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#include <iterator>
#include <vector>

#include "gtest/gtest.h"

#include "takram/algorithm/leaf_iterator_iterator.h"

namespace takram {
namespace algorithm {

namespace {

using C = std::vector<int>;
using B = std::vector<C>;
using A = std::vector<B>;
using Iterator = LeafIteratorIterator<A::iterator, B::iterator, C::iterator>;

}  // namespace

TEST(LeafIteratorIteratorTest, Traversing) {
  {
    A a;
    auto begin = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(begin, end);
  } {
    int i{};
    A a{{{++i, ++i}, {++i, ++i}}, {{++i, ++i}, {++i, ++i}}};
    auto itr = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_NE(itr, end);
    int j{};
    for (; itr != end; ++itr) {
      ASSERT_EQ(*itr, ++j);
    }
    ASSERT_EQ(itr, end);
  } {
    int i{};
    A a{{}, {{}, {++i}, {}}, {}, {{}, {++i}, {}}, {}};
    auto itr = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_NE(itr, end);
    int j{};
    for (; itr != end; ++itr) {
      ASSERT_EQ(*itr, ++j);
    }
    ASSERT_EQ(itr, end);
  } {
    A a{{{}}};
    auto itr = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(itr, end);
  } {
    A a{{{}}, {{}}};
    auto itr = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(itr, end);
  }
}

TEST(LeafIteratorIteratorTest, Distance) {
  {
    A a;
    auto begin = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(begin, end);
    ASSERT_EQ(std::distance(begin, end), 0);
  } {
    int i{};
    A a{{{++i, ++i}, {++i, ++i}}, {{}}, {{++i, ++i}, {++i, ++i}}};
    auto begin = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(std::distance(begin, end), i);
  } {
    int i{};
    A a{{}, {{}, {++i}, {}}, {}, {{}, {++i}, {}}, {}};
    auto begin = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(std::distance(begin, end), i);
  } {
    A a{{{}}};
    auto begin = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(std::distance(begin, end), 0);
  } {
    A a{{{}}, {{}}};
    auto begin = Iterator(a.begin(), a.end());
    auto end = Iterator(a.end(), a.end());
    ASSERT_EQ(std::distance(begin, end), 0);
  }
}

}  // namespace algorithm
}  // namespace takram
