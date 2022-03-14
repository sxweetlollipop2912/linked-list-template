#include <iostream>

#include "DoublyLinkedList.h"

void output(const List<int>& list) {
  std::cout << "<< OUTPUT <<\n";
  std::cout << "size: " << list.size() << '\n';
  std::cout << "Is list empty: " << list.is_empty() << '\n';
  std::cout << "front: " << list.front() << '\n';
  std::cout << "back: " << list.back() << '\n';
  for (int i = 0; i < list.size(); i++) std::cout << list.at(i) << ' ';
  std::cout << '\n';
  for (int i = 0; i < list.size(); i++) std::cout << list[i] << ' ';
  std::cout << '\n';
  for (auto it = list.begin(); it != list.end(); ++it)
    std::cout << (*it) << ' ';
  std::cout << "\n\n";
}


int main(int argc, const char* argv[]) {
  List<int> list{1, 2, 3};
  output(list);

  list.push_front(0);
  list.push_back(5);
  output(list);

  list.insert_at(4, 4);
  output(list);

  list.insert_at(list.size(), 6);
  output(list);

  list.insert_at(0, -1);
  output(list);

  list.remove_at(0);
  output(list);

  list.remove_at(list.size() - 1);
  output(list);

  list.remove_at(1);
  output(list);

  list.push_back(4);
  output(list);

  std::cout << "Is 5 an element: " << (list.find(5) != nullptr)
            << (list.find_last(5) != nullptr) << "\n\n";
  std::cout << "Is 5 an element: "
            << (list.find(5, list.begin(), list.find(4)) != nullptr)
            << (list.find_last(5, list.begin(), list.find_last(4)) != nullptr)
            << "\n\n";

  list.reverse();
  output(list);

  std::cout << "Is 5 an element: " << (list.find(5) != nullptr)
            << (list.find_last(5) != nullptr) << "\n\n";

  list.at(0) = 100;
  output(list);

  std::cout << "Is 5 an element: " << (list.find(5) != nullptr)
            << (list.find_last(5) != nullptr) << "\n\n";

  list.clear();
  list.push_back(1);
  output(list);

  list.remove_at(0);
  // CRASH at list.front()
  // output(list);
}
