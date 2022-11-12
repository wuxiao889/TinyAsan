#include <malloc.h>
#include <bits/stdc++.h>
#include <ext/pool_allocator.h>
using namespace std;

static long long countNew = 0;
static long long timesNew = 0;

void* operator new(size_t size){
  // cout << "global new " << size << endl;
  countNew += size;
  ++timesNew;
  return malloc(size);
}

// using 可以和模板搭配， typedef不可以
template <typename T>
using listPool = list<T,__gnu_cxx::__pool_alloc<T>>;

template <typename T>
void test_new(T container, int cnt){
  // clock_t timeStart = clock();
  countNew = 0;
  timesNew = 0;
  for(int i = 0 ; i < cnt; ++i)
    container.push_back(i);
  cout << "::countNew= " << ::countNew << endl;
  cout << "::timesNew= " << ::timesNew << endl;
  // cout << "using time= " << clock() - timeStart << endl;
}

int main(){
  // listPool<int> list;
  // list.push_back(1);
  // 慢了一点，因为池化复杂，new_allocator简单更快
  malloc_stats();
  test_new(listPool<int>(),1e6);
  // test_new(list<int>(),1e6);
  malloc_stats();
  test_new(listPool<int>(),1e6);
  // test_new(list<int>(),1e6);
  malloc_stats();
  return 0;
}

// 减少了cookie
// ::countNew= 25087984
// ::timesNew= 122
// ::countNew= 24000000
// ::timesNew= 1000000


// Arena 0:
// system bytes     =     135168
// in use bytes     =      73376
// Total (incl. mmap):
// system bytes     =     135168
// in use bytes     =      73376
// max mmap regions =          0
// max mmap bytes   =          0
// ::countNew= 25087984
// ::timesNew= 122
// Arena 0:
// system bytes     =    2412544
// in use bytes     =    2281104
// Total (incl. mmap):
// system bytes     =   25378816
// in use bytes     =   25247376
// max mmap regions =         40
// max mmap bytes   =   22966272

// Arena 0:
// system bytes     =     135168
// in use bytes     =      73376
// Total (incl. mmap):
// system bytes     =     135168
// in use bytes     =      73376
// max mmap regions =          0
// max mmap bytes   =          0
// ::countNew= 24000000
// ::timesNew= 1000000
// Arena 0:
// system bytes     =   32169984
// in use bytes     =      74640
// Total (incl. mmap):
// system bytes     =   32169984
// in use bytes     =      74640
// max mmap regions =          0
// max mmap bytes   =          0