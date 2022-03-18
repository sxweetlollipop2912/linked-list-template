#include <iostream>
#include <string>
#include <vector>
#include <list>
#include "DoublyLinkedList.h"

void output(const List<int>& list) {
  std::cout << "<< OUTPUT <<\n";
  std::cout << "size: " << list.size() << '\n';
  std::cout << "Is list empty: " << list.empty() << '\n';
  std::cout << "front: " << list.front() << '\n';
  std::cout << "back: " << list.back() << '\n';
  for (int i = 0; i < list.size(); i++) std::cout << list[i] << ' ';
  std::cout << '\n';
  for (auto it = list.begin(); it != list.end(); ++it)
    std::cout << (*it) << ' ';

  std::cout << "\n\n";
}

struct coord {
  int x, y;
};

int main(int argc, const char* argv[]) {
  std::list<int> l{3,2,1};
  l.sort();
  
  List<int>&& list{1, 2, 3};
  std::cout << list.size() << ' ' << std::distance(list.begin(), list.end())
            << '\n';
  // 1 2 3
  output(list);

  list.push_front(0);
  list.push_back(5);
  // 0 1 2 3 5
  output(list);

  list.insert_at(4, 4);
  list.insert_at(list.size(), 6);
  list.insert_at(0, -1);
  // -1 0 1 2 3 4 5 6
  output(list);
  
  list.remove_at(0);
  list.remove_at(list.size() - 1);
  // 0 1 2 3 4 5
  output(list);

  list.remove_at(1);
  list.push_back(4);
  std::cout << "----------------------";
  // 0 2 3 4 5 4
  output(list);
  // true true
  std::cout << "Is 5 an element: " << (list.find(5) != list.end())
            << (list.find_last(5) != list.end()) << "\n\n";
  // false true
  std::cout << "Is 5 an element: "
            << (list.find(5, list.begin(), list.find(4)) != list.end())
            << (list.find_last(5, list.begin(), list.find_last(4)) != nullptr)
            << "\n\n";

  list.reverse();
  list.at(0) = 100;
  // 100 5 4 3 2 0
  output(list);

  // true true
  std::cout << "Is 5 an element: " << (list.find(5) != list.end())
            << (list.find_last(5) != list.end()) << "\n\n";

  list.clear();
  list.push_back(1);
  // 1
  output(list);

  list.remove_at(0);
  // empty
  // error at list.front()
  // output(list);
  
  list = { 1,2,3,4,5,6 };
  // 1 2 3 4 5 6
  output(list);
  
  list.pop_front();
  list.pop_back();
  // 2 3 4 5
  output(list);
  
  list.insert(list.begin(), 1);
  list.insert(++list.begin(), 3);
  list.insert(list.end(), 6);
  // 1 3 2 3 4 5 6
  output(list);
  
  list.insert(list.begin(), 2, 0);
  list.insert(list.end(), 3, 7);
  // 0 0 1 3 2 3 4 5 6 7 7 7
  output(list);
  
  list.insert(--list.end(), list.begin(), ++(++list.begin()));
  // 0 0 1 3 2 3 4 5 6 7 7 0 0 7
  output(list);
  
  list.remove(3);
  // 0 0 1 2 4 5 6 7 7 0 0 7
  output(list);
  
  list.remove_if([](int x) { return x == 7; });
  // 0 0 1 2 4 5 6 0 0
  output(list);
  
  list.resize(11, 10);
  // 0 0 1 2 4 5 6 0 0 10 10
  output(list);
  
  list.unique([] (int x, int y) { return x == y; });
  // 0 1 2 4 5 6 0 10
  output(list);
  
  list.remove_at(6);
  // 0 1 2 4 5 6 10
  output(list);
  
  List<int> l2 = { 3,7,8,9,11 };
  list.merge(l2, [](int x, int y) { return x < y; });
  // 0 1 2 3 4 5 6 7 8 9 10 11
  output(list);
  // empty
  // error at l2.back()
  // output(l2);
  
  l2.assign(10, 1);
  // 1 1 1 1 1 1 1 1 1 1
  output(l2);
  
  l2.assign(++list.begin(), list.end());
  // 1 2 3 4 5 6 7 8 9 10 11
  output(l2);
  
  l2.assign({4, 3, 1, 2, 7, 9, 8, 6, 5});
  l2.sort([](int x, int y) { return x > y; });
  // 1 2 3 4 5 6 7 8 9
  output(l2);
  
  l2.filter([](const int x) { return x % 3 == 0; });
  // 9 6 3
  output(l2);

  List<std::string> ll{"acd", "cde", "acc"};
  // false
  std::cout << (ll.find_if([](const std::string& s) -> bool {
    return s[0] == 'b';
  }) != ll.end())
            << '\n';
  // 2
  std::cout << (ll.count_if([](const std::string& s) -> bool {
    return s[0] == 'a';
  })) << '\n';

  List<int> ll2{2, 4, 6, 8, 10};
  // true
  std::cout << ll2.all_of([](const int& value) { return value % 2 == 0; })
            << '\n';
  // false
  std::cout << ll2.any_of([](const int& value) { return value % 2 != 0; })
            << '\n';
  // true
  std::cout << ll2.none_of([](const int& value) { return value % 2 != 0; })
            << '\n';
}
