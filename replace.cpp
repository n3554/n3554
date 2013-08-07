#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> vec(10, 1);
  
  vec[5] = 7;

  std::replace(std::seq, vec.begin(), vec.end(), 7, 13);

  std::cout << "replace result:   ";
  for(int i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;

  std::replace_if(std::par, vec.begin(), vec.end(), [](int x){return x == 13;}, 7);

  std::cout << "replace_if result: ";
  for(int i = 0; i < vec.size(); ++i)
  {
    std::cout << vec[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

