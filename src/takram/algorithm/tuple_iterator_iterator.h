//
//  takram/algorithm/tuple_iterator_iterator.h
//
//  The MIT License
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

#pragma once
#ifndef TAKRAM_ALGORITHM_TUPLE_ITERATOR_ITERATOR_H_
#define TAKRAM_ALGORITHM_TUPLE_ITERATOR_ITERATOR_H_

#include <cstddef>
#include <iterator>
#include <tuple>
#include <utility>

namespace takram {
namespace algorithm {

template <class... Iterators>
class TupleIteratorIterator final
    : public std::iterator<std::forward_iterator_tag,
                           std::tuple<typename Iterators::reference...>> {
 private:
  using Type = std::tuple<typename Iterators::reference...>;
  using Pointer = Type *;

  template <std::size_t... Indexes>
  struct Equals;
  template <std::size_t... Indexes>
  struct Increment;

 public:
  TupleIteratorIterator();
  explicit TupleIteratorIterator(Iterators... iterators);

  // Copy semantics
  TupleIteratorIterator(const TupleIteratorIterator&) = default;
  TupleIteratorIterator& operator=(const TupleIteratorIterator&) = default;

  // Comparison
  template <class... Iters>
  friend bool operator==(const TupleIteratorIterator<Iters...>& lhs,
                         const TupleIteratorIterator<Iters...>& rhs);
  template <class... Iters>
  friend bool operator!=(const TupleIteratorIterator<Iters...>& lhs,
                         const TupleIteratorIterator<Iters...>& rhs);

  // Iterator
  Type operator*() const;
  Pointer operator->() const { return &operator*(); }
  TupleIteratorIterator& operator++();
  const TupleIteratorIterator& operator++(int);

 private:
  template <std::size_t... Indexes>
  bool equals(const TupleIteratorIterator& other,
              std::index_sequence<Indexes...>) const;
  template <std::size_t... Indexes>
  Type derefer(std::index_sequence<Indexes...>) const;
  template <std::size_t... Indexes>
  void increment(std::index_sequence<Indexes...>);

 private:
  std::tuple<Iterators...> iterators_;
};

#pragma mark -

template <class... Iterators>
inline TupleIteratorIterator<Iterators...>::TupleIteratorIterator() {}

template <class... Iterators>
inline TupleIteratorIterator<Iterators...>::TupleIteratorIterator(
    Iterators... iterators)
    : iterators_(iterators...) {}

#pragma mark Comparison

template <class... Iterators>
inline bool operator==(const TupleIteratorIterator<Iterators...>& lhs,
                       const TupleIteratorIterator<Iterators...>& rhs) {
  return lhs.equals(rhs, std::make_index_sequence<sizeof...(Iterators)>());
}

template <class... Iterators>
inline bool operator!=(const TupleIteratorIterator<Iterators...>& lhs,
                       const TupleIteratorIterator<Iterators...>& rhs) {
  return !(lhs == rhs);
}

template <class... Iterators>
template <std::size_t... Indexes>
inline bool TupleIteratorIterator<Iterators...>::equals(
    const TupleIteratorIterator& other,
    std::index_sequence<Indexes...>) const {
  return Equals<Indexes...>()(*this, other);
}

#pragma mark Iterator

template <class... Iterators>
inline typename TupleIteratorIterator<Iterators...>::Type
    TupleIteratorIterator<Iterators...>::operator*() const {
  return derefer(std::make_index_sequence<sizeof...(Iterators)>());
}

template <class... Iterators>
template <std::size_t... Indexes>
inline typename TupleIteratorIterator<Iterators...>::Type
    TupleIteratorIterator<Iterators...>::derefer(
        std::index_sequence<Indexes...>) const {
  return std::tie(*std::get<Indexes>(iterators_)...);
}

template <class... Iterators>
template <std::size_t... Indexes>
inline void TupleIteratorIterator<Iterators...>::increment(
    std::index_sequence<Indexes...>) {
  Increment<Indexes...>()(this);
}

template <class... Iterators>
inline TupleIteratorIterator<Iterators...>&
    TupleIteratorIterator<Iterators...>::operator++() {
  increment(std::make_index_sequence<sizeof...(Iterators)>());
  return *this;
}

template <class... Iterators>
inline const TupleIteratorIterator<Iterators...>&
    TupleIteratorIterator<Iterators...>::operator++(int) {
  TupleIteratorIterator result(*this);
  operator++();
  return result;
}

#pragma mark -

template <class... Iterators>
template <std::size_t Index, std::size_t... Indexes>
struct TupleIteratorIterator<Iterators...>::Equals<Index, Indexes...> {
  using Iterator = TupleIteratorIterator;
  bool operator()(const Iterator& a, const Iterator& b) {
    return ((std::get<Index>(a.iterators_) == std::get<Index>(b.iterators_)) ||
            Equals<Indexes...>()(a, b));
  }
};

template <class... Iterators>
template <std::size_t Index>
struct TupleIteratorIterator<Iterators...>::Equals<Index> {
  using Iterator = TupleIteratorIterator;
  bool operator()(const Iterator& a, const Iterator& b) {
    return std::get<Index>(a.iterators_) == std::get<Index>(b.iterators_);
  }
};

template <class... Iterators>
template <std::size_t Index, std::size_t... Indexes>
struct TupleIteratorIterator<Iterators...>::Increment<Index, Indexes...> {
  using Iterator = TupleIteratorIterator;
  void operator()(Iterator *iterator) {
    ++std::get<Index>(iterator->iterators_);
    Increment<Indexes...>()(iterator);
  }
};

template <class... Iterators>
template <std::size_t Index>
struct TupleIteratorIterator<Iterators...>::Increment<Index> {
  using Iterator = TupleIteratorIterator;
  void operator()(Iterator *iterator) {
    ++std::get<Index>(iterator->iterators_);
  }
};

}  // namespace algorithm

using algorithm::TupleIteratorIterator;

}  // namespace takram

#endif  // TAKRAM_ALGORITHM_TUPLE_ITERATOR_ITERATOR_H_
