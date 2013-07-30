#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> vec(10);

  vec[0] = 1;
  vec[1] = 1;
  vec[2] = 1;
  vec[3] = 1;

  std::cout << "count: " << std::count(std::seq, vec.begin(), vec.end(), 1) << std::endl;

  std::cout << "count_if: " << std::count_if(std::par, vec.begin(), vec.end(), [](int x)
  {
    return x == 0;
  }) << std::endl;

  return 0;
}

