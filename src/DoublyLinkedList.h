#pragma once

#include <cstddef>
#include <functional>
#include <initializer_list>
#include <iterator>
#include <stdexcept>
#include <string>

#include "Node.h"

template <class T>
class List {
 private:
  Node<T>*head, *tail;
  int list_size;

  /// Insert `next` next to `node`.
  /// Make sure `node` points to an element of this list.
  /// Remember to handle `next`->next and `next`->prev before calling this
  /// method. False if `node` is null or `next` is null while `node`->next is
  /// not null.
  bool insert_next(Node<T>*& node, Node<T>*& pNext) {
    if (!node || (node->next && !pNext)) return false;

    pNext->next = node->next;
    pNext->prev = node;
    pNext->prev->next = pNext;
    if (pNext->next) pNext->next->prev = pNext;

    if (node == this->tail) this->tail = pNext;

    ++this->list_size;

    return true;
  }
  /// Insert `prev` previous to `node`.
  /// Make sure `node` points to an element of this list.
  /// Remember to handle `prev`->next and `prev`->prev before calling this
  /// method. False if `node` is null or `prev` is null while `node`->prev is
  /// not null.
  bool insert_previous(Node<T>*& node, Node<T>*& pPrev) {
    if (!node || (node->prev && !pPrev)) return false;

    pPrev->next = node;
    pPrev->prev = node->prev;
    pPrev->next->prev = pPrev;
    if (pPrev->prev) pPrev->prev->next = pPrev;

    if (node == this->head) this->head = pPrev;

    ++this->list_size;

    return true;
  }
  /// Delete `node` from list.
  /// Make sure `node` points to an element of this list.
  /// False if `node` is null.
  bool remove(Node<T>*& node) {
    if (!node) return false;

    if (node->next) {
      node->next->prev = node->prev;
    }
    if (node->prev) {
      node->prev->next = node->next;
    }

    if (this->head == node) {
      head = node->next;
    }
    if (this->tail == node) {
      tail = node->prev;
    }

    --this->list_size;

    delete node;

    return true;
  }
  /// Exception(s): out of range
  Node<T>* get_node(const int& index) {
    if (index < 0 || index >= this->size()) {
      throw std::out_of_range("List of size " + std::to_string(this->size()) +
                              " is out of range at index " +
                              std::to_string(index) + ".");
    }

    iterator it;
    if (index < this->size() / 2) {
      it = iterator(head);
      for (int i = 1; i <= index; i++) ++it;
    } else {
      it = iterator(tail);
      for (int i = this->size() - 2; i >= index; i--) --it;
    }

    return it.ptr;
  }
  /// Exception(s): out of range
  const Node<T>* get_node(const int& index) const {
    if (index < 0 || index >= this->size()) {
      throw std::out_of_range("List of size " + std::to_string(this->size()) +
                              " is out of range at index " +
                              std::to_string(index) + ".");
    }

    iterator it;
    if (index < this->size() / 2) {
      it = iterator(head);
      for (int i = 1; i <= index; i++) ++it;
    } else {
      it = iterator(tail);
      for (int i = this->size() - 2; i >= index; i--) --it;
    }

    return it.ptr;
  }

 public:
  List() : head{nullptr}, tail{nullptr}, list_size{0} {}
  List(const std::initializer_list<T>& source)
      : head{nullptr}, tail{nullptr}, list_size{0} {
    for (auto it = source.begin(); it != source.end(); it++) {
      this->push_back(*it);
    }
  }
  List(const List<T>& source) : head{nullptr}, tail{nullptr}, list_size{0} {
    for (auto it = source.begin(); it != source.end(); it++) {
      this->push_back(*it);
    }
  }

  ~List() { this->clear(); }


  class iterator {
    friend class List;

   private:
    Node<T>* ptr;

   public:
    using iterator_category = std::bidirectional_iterator_tag;
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
    pointer operator->() { return &*(*this); }
    /// Prefix increment
    iterator& operator++() {
      ptr = ptr->next;
      return *this;
    }
    /// Postfix increment
    iterator operator++(int) {
      iterator tmp = *this;
      ptr = ptr->next;
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
      ptr = ptr->prev;
      return tmp;
    }
    iterator& operator=(Node<value_type>* const& p) {
      ptr = p;
      return *this;
    }
    bool operator==(const iterator& it) const { return it.ptr == ptr; }
    bool operator!=(const iterator& it) const { return it.ptr != ptr; }
  };
  
  
  class const_iterator {
    friend class List;

   private:
    const Node<T>* ptr;

   public:
    using iterator_category = std::bidirectional_iterator_tag;
    using value_type = T;
    using pointer = const T*;
    using reference = const T&;

    const_iterator() : ptr{nullptr} {}
    const_iterator(Node<value_type>* const& p) : ptr(p) {}
    const_iterator(const iterator other) : ptr(other.ptr) {}
    const_iterator(const_iterator& other) : ptr(other.ptr) {}
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
      ptr = ptr->next;
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
      ptr = ptr->prev;
      return tmp;
    }
    const_iterator& operator=(Node<value_type>* const& p) {
      ptr = p;
      return *this;
    }
    const_iterator& operator=(const iterator other) {
      ptr = other.ptr;
      std::cout << "CONST!\n";
      return *this;
    }
    bool operator==(const const_iterator& it) const { return it.ptr == ptr; }
    bool operator!=(const const_iterator& it) const { return it.ptr != ptr; }
  };


  bool is_empty() const { return this->size() == 0; }
  int length() const { return this->list_size; }
  int size() const { return this->list_size; }

  iterator begin() { return iterator(head); }
  const_iterator begin() const { return iterator(head); }
  iterator end() { return iterator(nullptr); }
  const_iterator end() const { return iterator(nullptr); }
  /// Exception(s): undefined behavior: null pointer dereference
  T front() const { return this->head->value; }
  /// Exception(s): undefined behavior: null pointer dereference
  T back() const { return this->tail->value; }
  /// Allows modifications, i.e. `list.at(0) = sth;`
  /// Exception(s): out of range
  T& at(const int& index) { return this->get_node(index)->value; }
  /// Does not allow modifications.
  /// Exception(s): out of range
  const T& at(const int& index) const { return this->get_node(index)->value; }
  /// Allows modifications, i.e. `list[0] = sth;`
  /// Exception(s): out of range
  T& operator[](const int& index) { return this->get_node(index)->value; }
  /// Does not allow modifications.
  /// Exception(s): out of range
  const T& operator[](const int& index) const {
    return this->get_node(index)->value;
  }

  void push_front(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (this->size() == 0) {
      head = tail = newNode;

      ++this->list_size;
    } else
      this->insert_previous(head, newNode);
  }
  void push_back(const T& value) {
    Node<T>* newNode = new Node<T>(value);
    if (this->size() == 0) {
      head = tail = newNode;

      ++this->list_size;
    } else
      this->insert_next(tail, newNode);
  }
  /// `value` will be at `index` in resulting list.
  /// Exception(s): out of range
  void insert_at(const int& index, const T& value) {
    if (index == 0) {
      push_front(value);
    } else if (index == this->size()) {
      push_back(value);
    } else {
      Node<T>*newNode = new Node<T>(value), *prev = this->get_node(index - 1);

      this->insert_next(prev, newNode);
    }
  }
  /// Exception(s): out of range
  void remove_at(const int& index) {
    auto node = this->get_node(index);
    this->remove(node);
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  const_iterator find(const T& value, const iterator& begin = nullptr,
                const iterator& end = nullptr) const {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end && (*it) != value; ++it)
      ;

    if (it == end)
      return iterator(nullptr);
    else
      return it;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  iterator find(const T& value, const iterator& begin = nullptr,
                const iterator& end = nullptr) {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end && (*it) != value; ++it)
      ;

    if (it == end)
      return iterator(nullptr);
    else
      return it;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  const_iterator find_if(std::function<bool(const T&)> func,
                   const iterator& begin = nullptr,
                   const iterator& end = nullptr) const {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end && !func(*it); ++it)
      ;

    if (it == end)
      return iterator(nullptr);
    else
      return it;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  iterator find_if(std::function<bool(const T&)> func,
                   const iterator& begin = nullptr,
                   const iterator& end = nullptr) {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end && !func(*it); ++it)
      ;

    if (it == end)
      return iterator(nullptr);
    else
      return it;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  int count_if(std::function<bool(const T&)> func,
               const iterator& begin = nullptr,
               const iterator& end = nullptr) const {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    int counter = 0;
    for (; it != end; ++it) counter += func(*it);
    return counter;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  bool all_of(std::function<bool(const T&)> func,
              const iterator& begin = nullptr,
              const iterator& end = nullptr) const {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end; ++it)
      if (!func(*it)) return false;
    return true;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  bool any_of(std::function<bool(const T&)> func,
              const iterator& begin = nullptr,
              const iterator& end = nullptr) const {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end; ++it)
      if (func(*it)) return true;
    return false;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  bool none_of(std::function<bool(const T&)> func,
               const iterator& begin = nullptr,
               const iterator& end = nullptr) const {
    auto it = begin == nullptr ? iterator(this->head) : begin;
    for (; it != end; ++it)
      if (func(*it)) return false;
    return true;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  const_iterator find_last(const T& value, const iterator& begin = nullptr,
                     const iterator& end = nullptr) const {
    auto rbegin = end == nullptr ? iterator(tail) : end;
    if (end != nullptr) --rbegin;

    auto rend = begin == nullptr ? iterator(head) : begin;
    --rend;

    auto it = rbegin;
    for (; it != rend && (*it) != value; --it)
      ;

    if (it == rend)
      return iterator(nullptr);
    else
      return it;
  }
  /// Make sure `begin` and `end` both point to elements in the same list.
  /// Exception(s): undefined behavior: null pointer dereference
  iterator find_last(const T& value, const iterator& begin = nullptr,
                     const iterator& end = nullptr) {
    auto rbegin = end == nullptr ? iterator(tail) : end;
    if (end != nullptr) --rbegin;

    auto rend = begin == nullptr ? iterator(head) : begin;
    --rend;

    auto it = rbegin;
    for (; it != rend && (*it) != value; --it)
      ;

    if (it == rend)
      return iterator(nullptr);
    else
      return it;
  }
  
  void reverse() {
    if (!this->is_empty()) {
      for (Node<T>*node = head, *pNext = nullptr; node != nullptr;
           node = pNext) {
        pNext = node->next;
        node->next = node->prev;
        node->prev = pNext;
      }

      Node<T>* node = head;
      head = tail;
      tail = node;
    }
  }
  void clear() {
    if (!this->is_empty()) {
      for (Node<T>*node = head, *pNext = nullptr; node != nullptr;
           node = pNext) {
        pNext = node->next;
        delete node;
      }
    }

    head = tail = nullptr;
    this->list_size = 0;
  }
};
