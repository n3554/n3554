#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> s1{1, 2, 3};
  std::vector<int> vec{1, 2, 3, 4, 1, 2, 3, 4, 1, 2, 3, 4};

  std::cout << "find: " << std::find_end(std::seq, vec.begin(), vec.end(), s1.begin(), s1.end()) - vec.begin() << std::endl;

  std::vector<int> s2{2, 4, 6};

  std::cout << "find with pred: " << std::find_end(std::seq, vec.begin(), vec.end(), s2.begin(), s2.end(), [](int x, int y){
    return x == y / 2;
  }) - vec.begin() << std::endl;
 
  return 0;
}

