#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> x(10), y(10);

  std::generate(std::seq, x.begin(), x.end(), [](){
    return 7;
  });

  std::cout << "generate result: ";
  for(int i = 0; i < x.size(); ++i)
  {
    std::cout << x[i] << " ";
  }
  std::cout << std::endl;

  std::generate_n(std::par, y.begin(), y.size(), [](){
    return 13;
  });

  std::cout << "generate_n result: ";
  for(int i = 0; i < y.size(); ++i)
  {
    std::cout << y[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

