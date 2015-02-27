#include <iostream>
#include <cstdlib>
#include <time.h>

#include <thrust/host_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/sort.h>

#include "timer.h"

#ifdef _MSC_VER
#define thrust_parallel thrust::cpp::par
#else
#define thrust_parallel thrust::omp::par
#endif

struct Int2comp {
  bool operator()(const int2& a, const int2& b) {
    if (a.x == b.x)
      return a.y < b.y;
    return a.x < b.x;
  }
};

int main(void)
{
  srand(time(NULL));

  std::cout << "sizeof(int) = " << sizeof(int) << std::endl;
  std::cout << "sizeof(long long) = " << sizeof(long long) << std::endl;

  int N = 10000000;

  thrust::host_vector<int2> foo(N);
  thrust::host_vector<long long> bar(N);

  for (int i = 0; i < N; i++) {
    int a = rand();
    int b = rand();

    int2 pair_foo;
    pair_foo.x = a;
    pair_foo.y = b;

    long long pair_bar = ((long long)a << 32 | (long long)b);

    foo[i] = pair_foo;
    bar[i] = pair_bar;

    //std::cout << "(" << a << " , " << b << ")   ";
  }
  std::cout << std::endl;

  CPUTimer timer_foo;
  CPUTimer timer_bar;

  timer_foo.Start();
  thrust::sort(thrust_parallel, foo.begin(), foo.end(), Int2comp());
  timer_foo.Stop();

  std::cout << "Sort pairs:   " << timer_foo.getElapsed() << std::endl;
  //for (int i = 0; i < N; i++)
  //  std::cout << "(" << foo[i].x << " , " << foo[i].y << ")   ";
  //std::cout << std::endl;

  timer_bar.Start();
  thrust::sort(thrust_parallel, bar.begin(), bar.end());
  timer_bar.Stop();

  std::cout << "Sort codes:   " << timer_bar.getElapsed() << std::endl;
  //for (int i = 0; i < N; i++) {
  //  int a = bar[i] >> 32;
  //  int b = bar[i] & 0xffffffff;
  //  std::cout << "(" << a << " , " << b << ")   ";
  //}
  //std::cout << std::endl;


  return 0;
}
