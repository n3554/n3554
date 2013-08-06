#include <algorithm>
#include <vector>
#include <numeric>
#include <iostream>

int main()
{
  std::vector<int> src(10), dst(10);

  std::iota(src.begin(), src.end(), 0);

  std::move(std::seq, src.begin(), src.end(), dst.begin());

  std::cout << "move result:   ";
  for(int i = 0; i < dst.size(); ++i)
  {
    std::cout << dst[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}

