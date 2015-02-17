#include <thrust/host_vector.h>
#include <thrust/iterator/zip_iterator.h>
#include <thrust/remove.h>
#include <thrust/count.h>
#include <iostream>

// Given a set of vectors (all of same size), remove the entries corresponding
// to indexes tagged as 'false' in a vector of flags.
int main(void)
{
  const bool  active[] = { false, true, false, true, false, false };
  const float a[] = {0.3f, 0.5f, 0.4f, 0.1f, 0.2f, 0.7f};
  const int   b[] = {3, 5, 4, 1, 2, 7};
  const double c[] = {13, 15, 14, 11, 12, 17 };

  thrust::host_vector<bool>   h_active(active, active + 6);
  thrust::host_vector<float>  h_a(a, a + 6);
  thrust::host_vector<int>    h_b(b, b + 6);
  thrust::host_vector<double> h_c(c, c + 6);

  // define a tuple of the three vector's iterators
  typedef thrust::tuple<thrust::host_vector<float>::iterator, thrust::host_vector<int>::iterator, thrust::host_vector<double>::iterator> IteratorTuple;

  // define a zip iterator
  typedef thrust::zip_iterator<IteratorTuple> ZipIterator;

  ZipIterator zip_begin = thrust::make_zip_iterator(thrust::make_tuple(h_a.begin(), h_b.begin(), h_c.begin()));
  ZipIterator zip_end   = zip_begin + 6;

  // call remove if on the zipped ranges (logical_not predicate: remove entries tagged with 'false')
  ZipIterator new_end = thrust::remove_if(zip_begin, zip_end, h_active.begin(), thrust::logical_not<bool>());

  // call remove if on the zipped ranges (identity predicate: remove entries tagged with 'true')
  //ZipIterator new_end = thrust::remove_if(zip_begin, zip_end, h_active.begin(), thrust::identity<bool>());

  // Count number of flags set to 'true'
  int num_flags = thrust::count_if(h_active.begin(), h_active.end(), thrust::identity<bool>());
  std::cout << "Keep " << num_flags << " elements" << std::endl;

  // erase the removed elements
  //h_a.erase(thrust::get<0>(new_end.get_iterator_tuple()), h_a.end());
  //h_b.erase(thrust::get<1>(new_end.get_iterator_tuple()), h_b.end());
  //h_c.erase(thrust::get<2>(new_end.get_iterator_tuple()), h_c.end());

  h_a.resize(num_flags);
  h_b.resize(num_flags);
  h_c.resize(num_flags);

  // print out the contents of the vectors
  std::cout << "New a: " << std::endl;
  for(int i = 0; i < h_a.size(); ++i)
  {
    std::cout << h_a[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "New b: " << std::endl;
  for(int i = 0; i < h_b.size(); ++i)
  {
    std::cout << h_b[i] << " ";
  }
  std::cout << std::endl;

  std::cout << "New c: " << std::endl;
  for (int i = 0; i < h_c.size(); ++i)
  {
    std::cout << h_c[i] << " ";
  }
  std::cout << std::endl;

  return 0;
}
