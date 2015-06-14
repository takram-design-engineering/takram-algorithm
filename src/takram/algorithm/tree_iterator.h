//
//  takram/algorithm/tree_iterator.h
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
class TreeIterator;

#pragma mark -

// Terminating partial specialization
template <typename T, typename Iterator>
class TreeIterator<T, Iterator> final
    : public std::iterator<std::forward_iterator_tag, T> {
 public:
  TreeIterator();
  TreeIterator(Iterator begin, Iterator end);

  // Copy semantics
  TreeIterator(const TreeIterator& other) = default;
  TreeIterator& operator=(const TreeIterator& other) = default;

  // Comparison
  bool operator==(const TreeIterator& other) const;
  bool operator!=(const TreeIterator& other) const;

  // Iterator
  T& operator*() const;
  T * operator->() const { return &operator*(); }
  TreeIterator& operator++();
  TreeIterator operator++(int);

 private:
  Iterator current_;
};

#pragma mark -

// Recursive partial specialization
template <typename T, typename Iterator, typename... RestIterators>
class TreeIterator<T, Iterator, RestIterators...> final
    : public std::iterator<std::forward_iterator_tag, T> {
 public:
  TreeIterator();
  TreeIterator(Iterator begin, Iterator end);

  // Copy semantics
  TreeIterator(const TreeIterator& other) = default;
  TreeIterator& operator=(const TreeIterator& other) = default;

  // Comparison
  bool operator==(const TreeIterator& other) const;
  bool operator!=(const TreeIterator& other) const;

  // Iterator
  T& operator*() const;
  T * operator->() const { return &operator*(); }
  TreeIterator& operator++();
  TreeIterator operator++(int);

 private:
  void validate();

 private:
  Iterator current_;
  Iterator end_;
  TreeIterator<T, RestIterators...> rest_;
};

#pragma mark -

template <typename T, typename Iterator>
inline TreeIterator<T, Iterator>::TreeIterator() : current_() {}

template <typename T, typename Iterator>
inline TreeIterator<T, Iterator>
    ::TreeIterator(Iterator begin, Iterator end)
    : current_(begin) {}

template <typename T, typename Iterator, typename... RestIterators>
inline TreeIterator<T, Iterator, RestIterators...>
    ::TreeIterator()
    : current_(),
      end_() {}

template <typename T, typename Iterator, typename... RestIterators>
inline TreeIterator<T, Iterator, RestIterators...>
    ::TreeIterator(Iterator begin, Iterator end)
    : current_(begin),
      end_(end) {
  validate();
}

#pragma mark Comparison

template <typename T, typename Iterator>
inline bool TreeIterator<T, Iterator>
    ::operator==(const TreeIterator& other) const {
  return current_ == other.current_;
}

template <typename T, typename Iterator>
inline bool TreeIterator<T, Iterator>
    ::operator!=(const TreeIterator& other) const {
  return !operator==(other);
}

template <typename T, typename Iterator, typename... RestIterators>
inline bool TreeIterator<T, Iterator, RestIterators...>
    ::operator==(const TreeIterator& other) const {
  return current_ == other.current_ && rest_ == other.rest_;
}

template <typename T, typename Iterator, typename... RestIterators>
inline bool TreeIterator<T, Iterator, RestIterators...>
    ::operator!=(const TreeIterator& other) const {
  return !operator==(other);
}

#pragma mark Iterator

template <typename T, typename Iterator>
inline T& TreeIterator<T, Iterator>::operator*() const {
  return *current_;
}

template <typename T, typename Iterator, typename... RestIterators>
inline T& TreeIterator<T, Iterator, RestIterators...>::operator*() const {
  return *rest_;
}

template <typename T, typename Iterator>
inline TreeIterator<T, Iterator>&
    TreeIterator<T, Iterator>::operator++() {
  ++current_;
  return *this;
}

template <typename T, typename Iterator, typename... RestIterators>
inline TreeIterator<T, Iterator, RestIterators...>&
    TreeIterator<T, Iterator, RestIterators...>::operator++() {
  using RestIterator = TreeIterator<T, RestIterators...>;
  if (++rest_ == RestIterator(current_->end(), current_->end())) {
    ++current_;
    validate();
  }
  return *this;
}

template <typename T, typename Iterator, typename... RestIterators>
inline void TreeIterator<T, Iterator, RestIterators...>::validate() {
  using RestIterator = TreeIterator<T, RestIterators...>;
  if (current_ == end_) {
    // Set the rest iterator to the default-constructed value so that
    // it can be comparred with end iterators.
    rest_ = RestIterator();
    return;
  }
  rest_ = RestIterator(current_->begin(), current_->end());
  while (rest_ == RestIterator() && current_ != end_) {
    ++current_;
    if (current_ != end_) {
      rest_ = RestIterator(current_->begin(), current_->end());
    } else {
      // Same discussion above
      rest_ = RestIterator();
    }
  }
}

template <typename T, typename Iterator>
inline TreeIterator<T, Iterator>
    TreeIterator<T, Iterator>::operator++(int) {
  TreeIterator result(*this);
  operator++();
  return result;
}

template <typename T, typename Iterator, typename... RestIterators>
inline TreeIterator<T, Iterator, RestIterators...>
    TreeIterator<T, Iterator, RestIterators...>::operator++(int) {
  TreeIterator result(*this);
  operator++();
  return result;
}

}  // namespace algorithm

using algorithm::TreeIterator;

}  // namespace takram

#endif  // TAKRAM_ALGORITHM_TREE_ITERATOR_H_
