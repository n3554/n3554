#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> vec(10);
  std::vector<int> copy(10);

  std::iota(vec.begin(), vec.end(), 0);

  std::reverse(std::seq, vec.begin(), vec.end());

  std::cout << "reverse result: ";
  for(int i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;

  std::reverse_copy(std::par, vec.begin(), vec.end(), copy.begin());

  std::cout << "reverse_copy result: ";
  for(int i = 0; i < copy.size(); ++i)
  {
    std::cout << copy[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

