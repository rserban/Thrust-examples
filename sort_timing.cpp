#include <iostream>
#include <cstdlib>
#include <time.h>

#include <thrust/host_vector.h>
#include <thrust/execution_policy.h>
#include <thrust/system/omp/execution_policy.h>
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

  int N = 1000000;
  thrust::host_vector<int2> foo(N);
  thrust::host_vector<long long> bar(N);

  int M = 100;
  CPUTimer timer_foo;
  CPUTimer timer_bar;
  double avg_foo = 0;
  double avg_bar = 0;

  for (int run = 0; run < M; run++) {
    for (int i = 0; i < N; i++) {
      int a = rand();
      int b = rand();

      int2 pair_foo;
      pair_foo.x = a;
      pair_foo.y = b;

      long long pair_bar = ((long long)a << 32 | (long long)b);

      foo[i] = pair_foo;
      bar[i] = pair_bar;
    }

    timer_foo.Start();
    thrust::sort(thrust_parallel, foo.begin(), foo.end(), Int2comp());
    timer_foo.Stop();
    avg_foo += timer_foo.getElapsed();

    timer_bar.Start();
    thrust::sort(thrust_parallel, bar.begin(), bar.end());
    timer_bar.Stop();
    avg_bar += timer_bar.getElapsed();
  }

  std::cout << "Sort pairs:   " << avg_foo / M << std::endl;
  std::cout << "Sort codes:   " << avg_bar / M << std::endl;

  //for (int i = 0; i < N; i++)
  //  std::cout << "(" << foo[i].x << " , " << foo[i].y << ")   ";
  //std::cout << std::endl;

  //for (int i = 0; i < N; i++) {
  //  int a = bar[i] >> 32;
  //  int b = bar[i] & 0xffffffff;
  //  std::cout << "(" << a << " , " << b << ")   ";
  //}
  //std::cout << std::endl;


  return 0;
}
