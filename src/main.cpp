#include <iostream>
// #include <list>
#include <string>
#include <vector>

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

int main(int argc, const char* argv[]) {
  // MAPPPPPPPPPPPPPPPPPPP
  List<int> tmp_list{1, 2, 3, 4, 5};
  auto tmp_list2 = tmp_list.map<std::string>(
      [](const int& x) { return "a[" + std::to_string(x) + "], "; });
  tmp_list2.for_each([](const std::string& x) { std::cout << x; });
  auto tmp_list10 = tmp_list.map<std::string>(
      [](const int& x) -> std::string { return std::to_string(x); });
  for (const auto& x : tmp_list10) std::cout << x << ' ';
  std::cout << '\n';
  std::cout << tmp_list10.reduce(
      [](const std::string& x, const std::string& y) { return x + ", " + y; },
      "");
}