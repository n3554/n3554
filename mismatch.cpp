#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> x(10), y(10);

  y[7] = 1;

  std::cout << "mismatch: " << std::mismatch(std::seq, x.begin(), x.end(), y.begin()).first - x.begin() << std::endl;


  std::cout << "mismatch with predicate: " << std::mismatch(std::par, x.begin(), x.end(), y.begin(), [](int x, int y)
  {
    return (x / 2) == (y / 2);
  }).first - x.begin() << std::endl;


  return 0;
}

