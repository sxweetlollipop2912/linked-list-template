#pragma once
#define _SILENCE_ALL_CXX17_DEPRECATION_WARNINGS
#define _SILENCE_CXX17_ITERATOR_BASE_CLASS_DEPRECATION_WARNING

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>

#include "Node.h"


template <class T>
class List {
 public:
  class iterator;
  class const_iterator;

 private:
  iterator list_begin, list_end;
  int list_size;
  /// False if either `it` or `it_next` is `end()`` or null.
  bool insert_next(iterator& it, iterator& it_next) {
    if (it == this->end() || it == nullptr || it_next == this->end() ||
        it_next == nullptr)
      return false;

    Node<T>*& node = it.ptr;
    Node<T>*& p_next = it_next.ptr;

    p_next->next = node->next;
    p_next->prev = node;
    p_next->prev->next = p_next;
    p_next->next->prev = p_next;

    ++this->list_size;

    return true;
  }
  /// False if either `it` and `it_prev` is null or `it_prev` is `end()`.
  bool insert_previous(const iterator& it, const iterator& it_prev) {
    if (it.ptr == nullptr || it_prev == this->end() || it_prev.ptr == nullptr)
      return false;

    Node<T>* node = it.ptr;
    Node<T>* p_prev = it_prev.ptr;

    p_prev->next = node;
    p_prev->prev = node->prev;
    p_prev->next->prev = p_prev;
    if (p_prev->prev) p_prev->prev->next = p_prev;

    if (it == this->begin()) list_begin = p_prev;

    ++this->list_size;

    return true;
  }
  /// Delete the node at `it` tfrom list.
  /// Make sure `it` belongs to this list.
  /// False if `it` is `end()` or the node at `it` is null.
  bool remove(iterator& it) {
    if (it == this->end() || it.ptr == nullptr) return false;

    Node<T>* node = it.ptr;

    if (node->next) {
      node->next->prev = node->prev;
    }
    if (node->prev) {
      node->prev->next = node->next;
    }

    if (this->begin() == it) {
      ++list_begin;
    }

    --this->list_size;

    delete node;
    it.ptr = nullptr;

    return true;
  }
  /// Exception(s): out of range
  iterator get_iterator(const int& index) {
    if (index < 0 || index >= this->size()) {
      throw std::out_of_range("List of size " + std::to_string(this->size()) +
                              " is out of range at index " +
                              std::to_string(index) + ".");
    }

    iterator it;
    if (index < this->size() / 2) {
      it = this->begin();
      for (int i = 1; i <= index; i++) ++it;
    } else {
      it = this->end();
      for (int i = this->size() - 1; i >= index; i--) --it;
    }

    return it;
  }
  /// Exception(s): out of range
  const_iterator get_iterator(const int& index) const {
    if (index < 0 || index >= this->size()) {
      throw std::out_of_range("List of size " + std::to_string(this->size()) +
                              " is out of range at index " +
                              std::to_string(index) + ".");
    }

    const_iterator it;
    if (index < this->size() / 2) {
      it = this->begin();
      for (int i = 1; i <= index; i++) ++it;
    } else {
      it = this->end();
      for (int i = this->size() - 1; i >= index; i--) --it;
    }

    return it;
  }

 public:
  List() : list_size{0} {
    Node<T>* end_node = new Node<T>();
    list_begin = list_end = end_node;
  }
  List(const std::initializer_list<T>& source) : list_size{0} {
    Node<T>* end_node = new Node<T>();
    list_begin = list_end = end_node;
    for (const auto& x : source) this->push_back(x);
  }
  List(const List<T>& source) : list_size{0} {
    Node<T>* end_node = new Node<T>();
    list_begin = list_end = end_node;

    for (const auto& x : source) this->push_back(x);
  }

  ~List() {
    this->clear();
    delete list_end.ptr;
  }

  bool empty() const { return this->size() == 0; }
  int length() const { return this->list_size; }
  int size() const { return this->list_size; }

  iterator begin() { return list_begin; }
  const_iterator begin() const { return list_begin; }
  iterator end() { return list_end; }
  const_iterator end() const { return list_end; }
  /// Allows modifications
  /// Exception(s): undefined behavior: null pointer dereference
  T& front() { return *this->begin(); }
  /// Does not allow modifications.
  /// Exception(s): undefined behavior: null pointer dereference
  const T& front() const { return *this->begin(); }
  /// Allows modifications
  /// Exception(s): undefined behavior: null pointer dereference
  T& back() {
    auto it = this->end();
    --it;
    return *it;
  }
  /// Does not allow modifications.
  /// Exception(s): undefined behavior: null pointer dereference
  const T& back() const {
    auto it = this->end();
    --it;
    return *it;
  }
  /// Allows modifications, i.e. `list[0] = sth;`
  /// Exception(s): out of range
  T& operator[](const int& index) { return *get_iterator(index); }
  /// Does not allow modifications.
  /// Exception(s): out of range
  const T& operator[](const int& index) const { return *get_iterator(index); }
  /// Allows modifications, i.e. `list.at(0) = sth;`
  /// Exception(s): out of range
  T& at(const int& index) { return (*this)[index]; }
  /// Does not allow modifications.
  /// Exception(s): out of range
  const T& at(const int& index) const { return (*this)[index]; }

  List<T>& push_front(const T& value) {
    Node<T>* new_node = new Node<T>(value);
    this->insert_previous(this->begin(), iterator(new_node));

    return *this;
  }
  List<T>& push_back(const T& value) {
    Node<T>* new_node = new Node<T>(value);
    this->insert_previous(this->end(), iterator(new_node));

    return *this;
  }
  /// `value` will be at `index` in resulting list.
  /// Exception(s): out of range
  List<T>& insert_at(const int& index, const T& value) {
    if (index == 0) {
      this->push_front(value);
    } else if (index == this->size()) {
      this->push_back(value);
    } else {
      auto it_new = iterator(new Node<T>(value));
      auto it_index = this->get_iterator(index - 1);

      this->insert_next(it_index, it_new);
    }

    return *this;
  }
  /// Exception(s): out of range
  List<T>& remove_at(const int& index) {
    auto it = this->get_iterator(index);
    this->remove(it);

    return *this;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  const_iterator find(const T& value, const const_iterator& begin = nullptr,
                      const const_iterator& end = nullptr) const {
    auto it = begin == nullptr ? const_iterator(this->begin()) : begin;
    auto current_end = end == nullptr ? const_iterator(this->end()) : end;

    for (; it != current_end && (*it) != value; ++it)
      ;

    if (it == current_end)
      return this->end();
    else
      return it;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  iterator find(const T& value, const iterator& begin = nullptr,
                const iterator& end = nullptr) {
    auto it = begin == nullptr ? iterator(this->begin()) : begin;
    auto current_end = end == nullptr ? iterator(this->end()) : end;

    for (; it != current_end && (*it) != value; ++it)
      ;

    if (it == current_end)
      return this->end();
    else
      return it;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  const_iterator find_if(std::function<bool(const T&)> func,
                         const const_iterator& begin = nullptr,
                         const const_iterator& end = nullptr) const {
    auto it = begin == nullptr ? const_iterator(this->begin()) : begin;
    auto current_end = end == nullptr ? const_iterator(this->end()) : end;

    for (; it != current_end && !func(*it); ++it)
      ;

    if (it == current_end)
      return this->end();
    else
      return it;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  iterator find_if(std::function<bool(const T&)> func,
                   const iterator& begin = nullptr,
                   const iterator& end = nullptr) {
    auto it = begin == nullptr ? this->begin() : begin;
    auto current_end = end == nullptr ? this->end() : end;

    for (; it != current_end && !func(*it); ++it)
      ;

    if (it == current_end)
      return this->end();
    else
      return it;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  int count_if(std::function<bool(const T&)> func,
               const const_iterator& begin = nullptr,
               const const_iterator& end = nullptr) const {
    auto it = begin == nullptr ? const_iterator(this->begin()) : begin;
    auto current_end = end == nullptr ? const_iterator(this->end()) : end;

    int counter = 0;
    for (; it != current_end; ++it) counter += func(*it);
    return counter;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  bool all_of(std::function<bool(const T&)> func,
              const const_iterator& begin = nullptr,
              const const_iterator& end = nullptr) const {
    auto it = begin == nullptr ? const_iterator(this->begin()) : begin;
    auto current_end = end == nullptr ? const_iterator(this->end()) : end;

    for (; it != current_end; ++it)
      if (!func(*it)) return false;
    return true;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  bool any_of(std::function<bool(const T&)> func,
              const const_iterator& begin = nullptr,
              const const_iterator& end = nullptr) const {
    auto it = begin == nullptr ? const_iterator(this->begin()) : begin;
    auto current_end = end == nullptr ? const_iterator(this->end()) : end;

    for (; it != current_end; ++it)
      if (func(*it)) return true;
    return false;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  bool none_of(std::function<bool(const T&)> func,
               const const_iterator& begin = nullptr,
               const const_iterator& end = nullptr) const {
    return !any_of(func, begin, end);
  }
  /// Exception(s): undefined behavior: null pointer dereference
  const_iterator find_last(const T& value,
                           const const_iterator& begin = nullptr,
                           const const_iterator& end = nullptr) const {
    auto rbegin = end == nullptr ? this->end() : end;
    --rbegin;

    auto rend = begin == nullptr ? const_iterator(this->begin()) : begin;
    --rend;

    auto it = rbegin;
    for (; it != rend && (*it) != value; --it)
      ;

    if (it == rend)
      return this->end();
    else
      return it;
  }
  /// Exception(s): undefined behavior: null pointer dereference
  iterator find_last(const T& value, const iterator& begin = nullptr,
                     const iterator& end = nullptr) {
    auto rbegin = end == nullptr ? this->end() : end;
    --rbegin;

    auto rend = begin == nullptr ? iterator(this->begin()) : begin;
    --rend;

    auto it = rbegin;
    for (; it != rend && (*it) != value; --it)
      ;

    if (it == rend)
      return this->end();
    else
      return it;
  }

  List<T>& reverse() {
    if (!this->empty()) {
      for (auto it = this->begin(), it_next = this->begin(); it != this->end();
           it = it_next) {
        it_next = it;
        ++it_next;

        Node<T>*& node = it.ptr;
        Node<T>*& p_next = it_next.ptr;

        node->next = node->prev;
        node->prev = p_next;
      }

      auto new_head = this->end();
      --new_head;
      auto new_tail = this->begin();

      new_head.ptr->prev = nullptr;
      new_tail.ptr->next = this->end().ptr;
      list_begin = new_head;
      list_end.ptr->prev = new_tail.ptr;
    }

    return *this;
  }
  List<T>& operator=(const List<T>& source) {
    this->clear();
    for (const auto& x : source) this->push_back(x);
    return *this;
  }
  List<T>& clear() {
    if (!this->empty()) {
      for (auto it = this->begin(); it != this->end();) {
        Node<T>* node = it.ptr;
        ++it;
        delete node;
      }
    }

    list_begin = list_end;
    list_end.ptr->prev = nullptr;
    this->list_size = 0;

    return *this;
  }
};

template <class T>
class List<T>::iterator
    : public std::iterator<std::bidirectional_iterator_tag, T, T, T*, T&> {
  friend class List;

 private:
  Node<T>* ptr;

 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = T;
  using value_type = T;
  using pointer = T*;
  using reference = T&;

  iterator() : ptr{nullptr} {}
  iterator(Node<value_type>* const& p) : ptr(p) {}
  reference operator*() const {
    reference value = ptr->value;
    return value;
  }
  /// ???? Copied from stackoverflow, not sure what it means.
  pointer operator->() const { return &*(*this); }
  /// Prefix increment
  iterator& operator++() {
    ptr = ptr->next;
    return *this;
  }
  /// Postfix increment
  iterator operator++(int) {
    iterator tmp = *this;
    ++(*this);
    return tmp;
  }
  /// Prefix decrement
  iterator& operator--() {
    ptr = ptr->prev;
    return *this;
  }
  /// Postfix decrement
  iterator operator--(int) {
    iterator tmp = *this;
    --(*this);
    return tmp;
  }
  iterator& operator=(Node<value_type>* const& p) {
    ptr = p;
    return *this;
  }

  bool operator==(const iterator& it) const { return it.ptr == ptr; }
  bool operator!=(const iterator& it) const { return it.ptr != ptr; }
};

template <class T>
class List<T>::const_iterator
    : public std::iterator<std::bidirectional_iterator_tag, T, T, T*, T&> {
  friend class List;

 private:
  const Node<T>* ptr;

 public:
  using iterator_category = std::bidirectional_iterator_tag;
  using difference_type = T;
  using value_type = T;
  using pointer = const T*;
  using reference = const T&;

  const_iterator() : ptr{nullptr} {}
  const_iterator(Node<value_type>* const& p) : ptr(p) {}
  const_iterator(const iterator& other) : ptr(other.ptr) {}
  const_iterator(const const_iterator& other) : ptr(other.ptr) {}
  reference operator*() const {
    reference value = ptr->value;
    return value;
  }
  /// ???? Copied from stackoverflow, not sure what it means.
  pointer operator->() const { return &*(*this); }
  /// Prefix increment
  const_iterator& operator++() {
    ptr = ptr->next;
    return *this;
  }
  /// Postfix increment
  const_iterator operator++(int) {
    const_iterator tmp = *this;
    ++(*this);
    return tmp;
  }
  /// Prefix decrement
  const_iterator& operator--() {
    ptr = ptr->prev;
    return *this;
  }
  /// Postfix decrement
  const_iterator operator--(int) {
    const_iterator tmp = *this;
    --(*this);
    return tmp;
  }
  const_iterator& operator=(Node<value_type>* const& p) {
    ptr = p;
    return *this;
  }
  const_iterator& operator=(const iterator& other) {
    ptr = other.ptr;
    return *this;
  }
  bool operator==(const const_iterator& it) const { return it.ptr == ptr; }
  bool operator!=(const const_iterator& it) const { return it.ptr != ptr; }
};