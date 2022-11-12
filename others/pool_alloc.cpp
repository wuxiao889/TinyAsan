#include <bits/stdc++.h>
#include <boost/type_index.hpp>
#include <ext/pool_allocator.h>
using namespace std;

template <typename Alloc>
void cookie_test(Alloc alloc, size_t n) {
  cout << boost::typeindex::type_id_with_cvr<Alloc>().pretty_name() << endl;
  typedef typename Alloc::value_type _valueType;
  _valueType *p1, *p2, *p3;
  p1 = alloc.allocate(n);
  p2 = alloc.allocate(n);
  p3 = alloc.allocate(n);
  printf("p1 = %p, p2 = %p, p3 = %p\n", p1, p2, p3);
  alloc.deallocate(p1,sizeof(_valueType));
  alloc.deallocate(p2,sizeof(_valueType));
  alloc.deallocate(p3,sizeof(_valueType));
}

int main() {
  // vector<string, __gnu_cxx::__pool_alloc<string>> vec;
  // vec.emplace_back("hello");
  // vec.push_back("world");
  // for(auto& s : vec) cout << s << endl;

  // cout << sizeof(__gnu_cxx::__pool_alloc<int>) << endl;
  // vector<string, __gnu_cxx::__pool_alloc<string>> vec;
  cookie_test(__gnu_cxx::__pool_alloc<int>(),1);
  cookie_test(std::allocator<int>(),1);
  return 0;
}

// __gnu_cxx::__pool_alloc<double>
// p1 = 0x55555556f320, p2 = 0x55555556f328, p3 = 0x55555556f330
// std::allocator<double>
// p1 = 0x55555556f470, p2 = 0x55555556f490, p3 = 0x55555556f4b0