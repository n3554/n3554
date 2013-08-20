#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> x(10), y(10);

  std::fill(std::seq, x.begin(), x.end(), 7);

  std::cout << "fill result: ";
  for(int i = 0; i < x.size(); ++i)
  {
    std::cout << x[i] << " ";
  }
  std::cout << std::endl;

  std::fill_n(std::par, y.begin(), y.size(), 13);

  std::cout << "fill_n result: ";
  for(int i = 0; i < y.size(); ++i)
  {
    std::cout << y[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

