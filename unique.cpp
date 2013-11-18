#include <algorithm>
#include <vector>
#include <numeric>
#include <functional>
#include <iostream>

int main()
{
  std::vector<int> vec(10, 1);
  std::vector<int> copy(10);

  std::iota(vec.begin(), vec.end(), 0);
  std::transform(vec.begin(), vec.end(), vec.begin(), [](int i)
  {
    return i / 2;
  });

  auto new_end = std::unique_copy(std::seq, vec.begin(), vec.end(), copy.begin(), std::equal_to<int>());
  copy.erase(new_end, copy.end());

  std::cout << "unique_copy result: ";
  for(int i = 0; i < copy.size(); ++i)
  {
    std::cout << copy[i] << " ";
  }
  std::cout << std::endl;

  new_end = std::unique(std::par, vec.begin(), vec.end(), std::equal_to<int>());
  vec.erase(new_end, vec.end());

  std::cout << "unique result: ";
  for(int i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

