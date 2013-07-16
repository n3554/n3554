#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> vec(10);

  vec[5] = 1;

  std::cout << "find: " << std::find(std::seq, vec.begin(), vec.end(), 1) - vec.begin() << std::endl;


  std::cout << "find_if: " << std::find_if(std::seq, vec.begin(), vec.end(), [](int x){
    return x == 1;
  }) - vec.begin() << std::endl;


  std::cout << "find_if_not: " << std::find_if_not(std::seq, vec.begin(), vec.end(), [](int x){
    return x == 0;
  }) - vec.begin() << std::endl;

  return 0;
}

