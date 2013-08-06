#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> x(10), y(10);

  std::iota(x.begin(), x.end(), 0);
  std::iota(y.begin(), y.end(), 10);

  std::swap_ranges(std::seq, x.begin(), x.end(), y.begin());

  std::cout << "swap_ranges result: " << std::endl;

  std::cout << "x: ";
  for(int i = 0; i < x.size(); ++i)
  {
    std::cout << x[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "y: ";
  for(int i = 0; i < y.size(); ++i)
  {
    std::cout << y[i] << " ";
  }
  std::cout << std::endl;

  std::cout << std::endl;

  std::swap_ranges(std::par, x.begin(), x.end(), y.begin());

  std::cout << "x: ";
  for(int i = 0; i < x.size(); ++i)
  {
    std::cout << x[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "y: ";
  for(int i = 0; i < y.size(); ++i)
  {
    std::cout << y[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

