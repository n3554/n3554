#include <algorithm>
#include <vector>
#include <iostream>

int main()
{
  std::vector<int> x(10), y(10);

  y[7] = 1;

  std::cout << "equal: " << std::equal(std::seq, x.begin(), x.end(), y.begin()) << std::endl;


  std::cout << "equal with predicate: " << std::equal(std::par, x.begin(), x.end(), y.begin(), [](int x, int y)
  {
    return (x / 2) == (y / 2);
  }) << std::endl;


  return 0;
}

