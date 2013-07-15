#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> vec(10);

  std::cout << "all_of: " << std::all_of(std::seq, vec.begin(), vec.end(), [](int x){
    return x == 0;
  });
  std::cout << std::endl;


  std::cout << "any_of: " << std::any_of(std::par, vec.begin(), vec.end(), [](int x){
    return x == 0;
  });
  std::cout << std::endl;


  std::cout << "none_of: " << std::none_of(vec.begin(), vec.end(), [](int x){
    return x == 0;
  });
  std::cout << std::endl;

  return 0;
}


