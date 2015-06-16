//
//  takram/algorithm/leaf_iterator.h
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

#pragma once
#ifndef TAKRAM_ALGORITHM_TREE_ITERATOR_H_
#define TAKRAM_ALGORITHM_TREE_ITERATOR_H_

#include <iterator>

namespace takram {
namespace algorithm {

// Primary template
template <typename T, typename... Iterators>
class LeafIterator;

#pragma mark -

// Terminating partial specialization
template <typename T, typename Iterator>
class LeafIterator<T, Iterator> final
    : public std::iterator<std::forward_iterator_tag, T> {
 public:
  LeafIterator();
  LeafIterator(Iterator begin, Iterator end);

  // Copy semantics
  LeafIterator(const LeafIterator& other) = default;
  LeafIterator& operator=(const LeafIterator& other) = default;

  // Comparison
  bool operator==(const LeafIterator& other) const;
  bool operator!=(const LeafIterator& other) const;

  // Iterator
  T& operator*() const;
  T * operator->() const { return &operator*(); }
  LeafIterator& operator++();
  LeafIterator operator++(int);

 private:
  Iterator current_;
};

#pragma mark -

// Recursive partial specialization
template <typename T, typename Iterator, typename... RestIterators>
class LeafIterator<T, Iterator, RestIterators...> final
    : public std::iterator<std::forward_iterator_tag, T> {
 public:
  LeafIterator();
  LeafIterator(Iterator begin, Iterator end);

  // Copy semantics
  LeafIterator(const LeafIterator& other) = default;
  LeafIterator& operator=(const LeafIterator& other) = default;

  // Comparison
  bool operator==(const LeafIterator& other) const;
  bool operator!=(const LeafIterator& other) const;

  // Iterator
  T& operator*() const;
  T * operator->() const { return &operator*(); }
  LeafIterator& operator++();
  LeafIterator operator++(int);

 private:
  void validate();

 private:
  Iterator current_;
  Iterator end_;
  LeafIterator<T, RestIterators...> rest_;
};

#pragma mark -

template <typename T, typename Iterator>
inline LeafIterator<T, Iterator>::LeafIterator() : current_() {}

template <typename T, typename Iterator>
inline LeafIterator<T, Iterator>
    ::LeafIterator(Iterator begin, Iterator end)
    : current_(begin) {}

template <typename T, typename Iterator, typename... RestIterators>
inline LeafIterator<T, Iterator, RestIterators...>
    ::LeafIterator()
    : current_(),
      end_() {}

template <typename T, typename Iterator, typename... RestIterators>
inline LeafIterator<T, Iterator, RestIterators...>
    ::LeafIterator(Iterator begin, Iterator end)
    : current_(begin),
      end_(end) {
  validate();
}

#pragma mark Comparison

template <typename T, typename Iterator>
inline bool LeafIterator<T, Iterator>
    ::operator==(const LeafIterator& other) const {
  return current_ == other.current_;
}

template <typename T, typename Iterator>
inline bool LeafIterator<T, Iterator>
    ::operator!=(const LeafIterator& other) const {
  return !operator==(other);
}

template <typename T, typename Iterator, typename... RestIterators>
inline bool LeafIterator<T, Iterator, RestIterators...>
    ::operator==(const LeafIterator& other) const {
  return (current_ == other.current_ ||
          (current_ == Iterator() && other.current_ == other.end_) ||
          (current_ == end_ && other.current_ == Iterator())) &&
         rest_ == other.rest_;
}

template <typename T, typename Iterator, typename... RestIterators>
inline bool LeafIterator<T, Iterator, RestIterators...>
    ::operator!=(const LeafIterator& other) const {
  return !operator==(other);
}

#pragma mark Iterator

template <typename T, typename Iterator>
inline T& LeafIterator<T, Iterator>::operator*() const {
  return *current_;
}

template <typename T, typename Iterator, typename... RestIterators>
inline T& LeafIterator<T, Iterator, RestIterators...>::operator*() const {
  return *rest_;
}

template <typename T, typename Iterator>
inline LeafIterator<T, Iterator>&
    LeafIterator<T, Iterator>::operator++() {
  ++current_;
  return *this;
}

template <typename T, typename Iterator, typename... RestIterators>
inline LeafIterator<T, Iterator, RestIterators...>&
    LeafIterator<T, Iterator, RestIterators...>::operator++() {
  using RestIterator = LeafIterator<T, RestIterators...>;
  if (++rest_ == RestIterator(current_->end(), current_->end())) {
    ++current_;
    validate();
  }
  return *this;
}

template <typename T, typename Iterator, typename... RestIterators>
inline void LeafIterator<T, Iterator, RestIterators...>::validate() {
  using RestIterator = LeafIterator<T, RestIterators...>;
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

template <typename T, typename Iterator>
inline LeafIterator<T, Iterator>
    LeafIterator<T, Iterator>::operator++(int) {
  LeafIterator result(*this);
  operator++();
  return result;
}

template <typename T, typename Iterator, typename... RestIterators>
inline LeafIterator<T, Iterator, RestIterators...>
    LeafIterator<T, Iterator, RestIterators...>::operator++(int) {
  LeafIterator result(*this);
  operator++();
  return result;
}

}  // namespace algorithm

using algorithm::LeafIterator;

}  // namespace takram

#endif  // TAKRAM_ALGORITHM_TREE_ITERATOR_H_
