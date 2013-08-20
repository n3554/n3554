#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> vec(10, 1);
  
  vec[5] = 7;

  auto new_end = std::remove(std::seq, vec.begin(), vec.end(), 7);
  vec.erase(new_end, vec.end());

  std::cout << "remove result:   ";
  for(int i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;

  vec.resize(10, 1);

  vec[5] = 7;

  new_end = std::remove_if(std::par, vec.begin(), vec.end(), [](int x){return x == 7;});
  vec.erase(new_end, vec.end());

  std::cout << "remove_if result: ";
  for(int i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

