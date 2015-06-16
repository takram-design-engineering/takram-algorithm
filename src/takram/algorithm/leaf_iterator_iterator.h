//
//  takram/algorithm/leaf_iterator_iterator.h
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
//  LIABILITY, WHETHER IN AN ACTION OF CONTRACTORT OR OTHERWISE, ARISING
//  FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
//  DEALINGS IN THE SOFTWARE.
//

#pragma once
#ifndef TAKRAM_ALGORITHM_LEAF_ITERATOR_ITERATOR_H_
#define TAKRAM_ALGORITHM_LEAF_ITERATOR_ITERATOR_H_

#include <iterator>

#include "takram/algorithm/variadic_template.h"

namespace takram {
namespace algorithm {

// Primary template
template <class... Iterators>
class LeafIteratorIterator;

#pragma mark -

// Terminating partial specialization
template <class Iterator>
class LeafIteratorIterator<Iterator> final
    : public std::iterator<
          std::forward_iterator_tag,
          typename Iterator::value_type> {
 private:
  using ValueType = typename Iterator::value_type;

 public:
  LeafIteratorIterator();
  LeafIteratorIterator(Iterator begin, Iterator end);

  // Copy semantics
  LeafIteratorIterator(const LeafIteratorIterator& other) = default;
  LeafIteratorIterator& operator=(const LeafIteratorIterator& other) = default;

  // Comparison
  bool operator==(const LeafIteratorIterator& other) const;
  bool operator!=(const LeafIteratorIterator& other) const;

  // Iterator
  ValueType& operator*() const;
  ValueType * operator->() const { return &operator*(); }
  LeafIteratorIterator& operator++();
  const LeafIteratorIterator operator++(int);

 private:
  Iterator current_;
};

#pragma mark -

// Recursive partial specialization
template <class Iterator, class... RestIterators>
class LeafIteratorIterator<Iterator, RestIterators...> final
    : public std::iterator<
          std::forward_iterator_tag,
          typename Last<Iterator, RestIterators...>::Type::value_type> {
 private:
  using ValueType = typename Last<Iterator, RestIterators...>::Type::value_type;

 public:
  LeafIteratorIterator();
  LeafIteratorIterator(Iterator begin, Iterator end);

  // Copy semantics
  LeafIteratorIterator(const LeafIteratorIterator& other) = default;
  LeafIteratorIterator& operator=(const LeafIteratorIterator& other) = default;

  // Comparison
  bool operator==(const LeafIteratorIterator& other) const;
  bool operator!=(const LeafIteratorIterator& other) const;

  // Iterator
  ValueType& operator*() const;
  ValueType * operator->() const { return &operator*(); }
  LeafIteratorIterator& operator++();
  const LeafIteratorIterator operator++(int);

 private:
  void validate();

 private:
  Iterator current_;
  Iterator end_;
  LeafIteratorIterator<RestIterators...> rest_;
};

#pragma mark -

template <class Iterator>
inline LeafIteratorIterator<Iterator>::LeafIteratorIterator() : current_() {}

template <class Iterator>
inline LeafIteratorIterator<Iterator>::LeafIteratorIterator(
    Iterator begin, Iterator end)
    : current_(begin) {}

template <class Iterator, class... RestIterators>
inline LeafIteratorIterator<Iterator, RestIterators...>::LeafIteratorIterator()
    : current_(),
      end_() {}

template <class Iterator, class... RestIterators>
inline LeafIteratorIterator<Iterator, RestIterators...>::LeafIteratorIterator(
    Iterator begin, Iterator end)
    : current_(begin),
      end_(end) {
  validate();
}

#pragma mark Comparison

template <class Iterator>
inline bool LeafIteratorIterator<Iterator>::operator==(
    const LeafIteratorIterator& other) const {
  return current_ == other.current_;
}

template <class Iterator>
inline bool LeafIteratorIterator<Iterator>::operator!=(
    const LeafIteratorIterator& other) const {
  return !operator==(other);
}

template <class Iterator, class... RestIterators>
inline bool LeafIteratorIterator<Iterator, RestIterators...>::operator==(
    const LeafIteratorIterator& other) const {
  return (current_ == other.current_ ||
          (current_ == Iterator() && other.current_ == other.end_) ||
          (current_ == end_ && other.current_ == Iterator())) &&
         rest_ == other.rest_;
}

template <class Iterator, class... RestIterators>
inline bool LeafIteratorIterator<Iterator, RestIterators...>::operator!=(
    const LeafIteratorIterator& other) const {
  return !operator==(other);
}

#pragma mark Iterator

template <class Iterator>
inline typename LeafIteratorIterator<Iterator>::ValueType&
    LeafIteratorIterator<Iterator>::operator*() const {
  return *current_;
}

template <class Iterator, class... RestIterators>
inline typename LeafIteratorIterator<Iterator, RestIterators...>::ValueType&
    LeafIteratorIterator<Iterator, RestIterators...>::operator*() const {
  return *rest_;
}

template <class Iterator>
inline LeafIteratorIterator<Iterator>&
    LeafIteratorIterator<Iterator>::operator++() {
  ++current_;
  return *this;
}

template <class Iterator, class... RestIterators>
inline LeafIteratorIterator<Iterator, RestIterators...>&
    LeafIteratorIterator<Iterator, RestIterators...>::operator++() {
  using RestIterator = LeafIteratorIterator<RestIterators...>;
  if (++rest_ == RestIterator(current_->end(), current_->end())) {
    ++current_;
    validate();
  }
  return *this;
}

template <class Iterator, class... RestIterators>
inline void LeafIteratorIterator<Iterator, RestIterators...>::validate() {
  using RestIterator = LeafIteratorIterator<RestIterators...>;
  if (current_ == end_) {
    rest_ = RestIterator();
    return;
  }
  rest_ = RestIterator(current_->begin(), current_->end());
  while (rest_ == RestIterator() && current_ != end_) {
    ++current_;
    if (current_ != end_) {
      rest_ = RestIterator(current_->begin(), current_->end());
    }
  }
}

template <class Iterator>
inline const LeafIteratorIterator<Iterator>
    LeafIteratorIterator<Iterator>::operator++(int) {
  LeafIteratorIterator result(*this);
  operator++();
  return result;
}

template <class Iterator, class... RestIterators>
inline const LeafIteratorIterator<Iterator, RestIterators...>
    LeafIteratorIterator<Iterator, RestIterators...>::operator++(int) {
  LeafIteratorIterator result(*this);
  operator++();
  return result;
}

}  // namespace algorithm

using algorithm::LeafIteratorIterator;

}  // namespace takram

#endif  // TAKRAM_ALGORITHM_LEAF_ITERATOR_ITERATOR_H_
