#include <algorithm>
#include <vector>

int main()
{
  std::vector<int> vec(10);

  std::for_each(std::par, vec.begin(), vec.end(), [](int x){
    std::cout << x << " ";
  });
  std::cout << std::endl;


  std::for_each(std::seq, vec.begin(), vec.end(), [](int x){
    std::cout << x << " ";
  });
  std::cout << std::endl;


  std::execution_policy exec;
  std::for_each(exec, vec.begin(), vec.end(), [](int x){
    std::cout << x << " ";
  });
  std::cout << std::endl;


  exec = std::seq;
  std::for_each(exec, vec.begin(), vec.end(), [](int x){
    std::cout << x << " ";
  });
  std::cout << std::endl;


  exec = std::par;
  std::for_each(exec, vec.begin(), vec.end(), [](int x){
    std::cout << x << " ";
  });
  std::cout << std::endl;


  return 0;
}

