//
//  tuple_iterator_iterator_test.cc
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
#include <list>
#include <numeric>
#include <vector>

#include "gtest/gtest.h"

#include "takram/algorithm/tuple_iterator_iterator.h"

namespace takram {
namespace algorithm {

namespace {

class T {
 public:
  T() = default;
  T(int value) : value(value) {}
  T(const T&) = delete;
  T& operator=(const T&) = delete;
  int value;
};

using A = std::vector<int>;
using B = std::list<float>;
using C = std::vector<T>;
using Iterator = TupleIteratorIterator<A::iterator, B::iterator, C::iterator>;

}  // namespace

TEST(TupleIteratorIteratorTest, Traversing) {

  {
std::vector<int> a(4);
std::vector<float> b(5);
std::vector<double> c(6);
std::iota(a.begin(), a.end(), 0);
std::iota(b.begin(), b.end(), 0);
std::iota(c.begin(), c.end(), 0);

using Iterator = TupleIteratorIterator<
    decltype(a)::iterator, decltype(b)::iterator, decltype(c)::iterator>;
auto itr = Iterator(std::begin(a), std::begin(b), std::begin(c));
const auto end = Iterator(std::end(a), std::end(b), std::end(c));
for (; itr != end; ++itr) {
  auto& a = std::get<0>(*itr);
  auto& b = std::get<1>(*itr);
  auto& c = std::get<2>(*itr);
  std::cout << a << " " << b << " " << c << std::endl;
}
  }










  A a(5);
  B b(5);
  C c(5);
  std::iota(a.begin(), a.end(), 0);
  std::iota(b.begin(), b.end(), 0);
  for (int i = 0; i < c.size(); ++i) {
    c.at(i).value = i;
  }
  auto itr = Iterator(std::begin(a), std::begin(b), std::begin(c));
  auto end = Iterator(std::end(a), std::end(b), std::end(c));
  ASSERT_NE(itr, end);
  int j{};
  for (; itr != end; ++itr) {
    auto& a = std::get<0>(*itr);
    auto& b = std::get<1>(*itr);
    auto& c = std::get<2>(*itr);
    ASSERT_EQ(a, j);
    ASSERT_EQ(b, j);
    ASSERT_EQ(c.value, j);
    ++j;
  }
  ASSERT_EQ(itr, end);
}

TEST(TupleIteratorIteratorTest, Distance) {
  {
    A a(5);
    B b(5);
    C c(5);
    const auto begin = Iterator(std::begin(a), std::begin(b), std::begin(c));
    const auto end = Iterator(std::end(a), std::end(b), std::end(c));
    ASSERT_EQ(std::distance(begin, end), 5);
  } {
    A a(5);
    const auto begin = TupleIteratorIterator<A::iterator>(std::begin(a));
    const auto end = TupleIteratorIterator<A::iterator>(std::end(a));
    ASSERT_EQ(std::distance(begin, end), 5);
  } {
    A a;
    B b;
    C c;
    const auto begin = Iterator(std::begin(a), std::begin(b), std::begin(c));
    const auto end = Iterator(std::end(a), std::end(b), std::end(c));
    ASSERT_EQ(begin, end);
    ASSERT_EQ(std::distance(begin, end), 0);
  } {
    A a(1);
    B b(2);
    C c(3);
    const auto begin = Iterator(std::begin(a), std::begin(b), std::begin(c));
    const auto end = Iterator(std::end(a), std::end(b), std::end(c));
    ASSERT_EQ(std::distance(begin, end), 1);
  }
}

}  // namespace algorithm
}  // namespace takram
