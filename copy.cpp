#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> src(10), dst(10);

  std::iota(src.begin(), src.end(), 0);

  std::copy(std::seq, src.begin(), src.end(), dst.begin());

  std::cout << "copy result:   ";
  for(int i = 0; i < dst.size(); ++i)
  {
    std::cout << dst[i] << " ";
  }
  std::cout << std::endl;

  std::copy_n(std::par, src.begin(), src.size(), dst.begin());

  std::cout << "copy_n result: ";
  for(int i = 0; i < dst.size(); ++i)
  {
    std::cout << dst[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

