#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> vec(10);

  vec[1] = 1;

  std::cout << "adjacent_find: " << std::adjacent_find(std::seq, vec.begin(), vec.end()) - vec.begin() << std::endl;


  std::cout << "adjacent_find with predicate: " << std::adjacent_find(std::par, vec.begin(), vec.end(), [](int x, int y)
  {
    return (x/2) == (y/2);
  }) - vec.begin() << std::endl;


  return 0;
}

