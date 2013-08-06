#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> src(10), dst(10);

  std::iota(src.begin(), src.end(), 0);

  std::transform(std::seq, src.begin(), src.end(), dst.begin(), std::negate<int>());

  std::cout << "unary transform result:  ";
  for(int i = 0; i < dst.size(); ++i)
  {
    std::cout << dst[i] << " ";
  }
  std::cout << std::endl;

  std::transform(std::par, src.begin(), src.end(), dst.begin(), dst.begin(), std::plus<int>());

  std::cout << "binary transform result: ";
  for(int i = 0; i < dst.size(); ++i)
  {
    std::cout << dst[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

