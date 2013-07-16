#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> v{0, 2, 3, 25, 5};
  std::vector<int> t1{3, 19, 10, 2};
 
  std::cout << "find_first_of: " <<  std::find_first_of(v.begin(), v.end(), t1.begin(), t1.end()) - v.begin() << std::endl;
  
  std::vector<int> t2{6, 38, 20, 4};
  std::cout << "find_first_of with pred: " <<  std::find_first_of(v.begin(), v.end(), t2.begin(), t2.end(), [](int x, int y){
    return x == (y / 2);
  }) - v.begin() << std::endl;
 
  return 0;
}


