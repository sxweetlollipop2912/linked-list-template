
#include <iostream>
#include <string>
#include <vector>

#include "DoublyLinkedList.h"

void output(const List<int>& list) {
  std::cout << "<< OUTPUT <<\n";
  std::cout << "size: " << list.size() << '\n';
  std::cout << "Is list empty: " << list.is_empty() << '\n';
  std::cout << "front: " << list.front() << '\n';
  std::cout << "back: " << list.back() << '\n';
  for (int i = 0; i < list.size(); i++) std::cout << list[i] << ' ';
  std::cout << '\n';
  for (auto it = list.begin(); it != list.end(); ++it)
    std::cout << (*it) << ' ';
  std::cout << "\n\n";
  
  // CRASH since list.begin() returns a const_iterator
  // (*list.begin()) = 100;
  // std::cout << (*list.begin()) << '\n';
}


int main(int argc, const char* argv[]) {
  List<int> list{1, 2, 3};
  output(list);

  list.push_front(0).push_back(5);
  output(list);

  list.insert_at(4, 4)
    .insert_at(list.size(), 6)
    .insert_at(0, -1);
  output(list);

  list.remove_at(0).remove_at(list.size() - 1);
  output(list);

  list.remove_at(1).push_back(4);
  std::cout << "----------------------";
  output(list);
  std::cout << "Is 5 an element: " << (list.find(5) != nullptr)
            << (list.find_last(5) != nullptr) << "\n\n";
  std::cout << "Is 5 an element: "
            << (list.find(5, list.begin(), list.find(4)) != nullptr)
            << (list.find_last(5, list.begin(), list.find_last(4)) != nullptr)
            << "\n\n";

  list.reverse().at(0) = 100;
  output(list);

  std::cout << "Is 5 an element: " << (list.find(5) != nullptr)
            << (list.find_last(5) != nullptr) << "\n\n";
  
  list.clear().push_back(1);
  output(list);

  list.remove_at(0);
  // CRASH at list.front()
  // output(list);
  
  list.push_back(0).push_back(1);
  auto l2 = list.clone().push_back(2);
  std::cout << "L1 back: " << list.back() << "\nL2 back: " << l2.back() << "\n\n";


  List<std::string> ll{"acd", "cde", "acc"};
  std::cout << (ll.find_if([](const std::string& s) -> bool {
    return s[0] == 'b';
  }) != nullptr)
            << '\n';
  std::cout << (ll.count_if([](const std::string& s) -> bool {
    return s[0] == 'a';
  })) << '\n';
  List<int> ll2{2, 4, 6, 8, 10};
  std::cout << ll2.all_of([](const int& value) { return value % 2 == 0; })
            << '\n';
  std::cout << ll2.any_of([](const int& value) { return value % 2 != 0; })
            << '\n';
  std::cout << ll2.none_of([](const int& value) { return value % 2 != 0; })
            << '\n';
}
