#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> vec(10, 1);
  std::vector<int> copy(10);
  
  vec[5] = 7;

  auto new_end = std::remove_copy(std::seq, vec.begin(), vec.end(), copy.begin(), 7);
  copy.erase(new_end, copy.end());

  std::cout << "remove_copy result: ";
  for(int i = 0; i < copy.size(); ++i)
  {
    std::cout << copy[i] << " ";
  }
  std::cout << std::endl;

  copy.resize(10);

  vec[5] = 7;

  new_end = std::remove_copy_if(std::par, vec.begin(), vec.end(), copy.begin(), [](int x){return x == 7;});
  copy.erase(new_end, copy.end());

  std::cout << "remove_if result:   ";
  for(int i = 0; i < copy.size(); ++i)
  {
    std::cout << copy[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

