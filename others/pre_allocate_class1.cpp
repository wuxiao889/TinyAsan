#include <stdlib.h>
#include <iostream>
#include <new>

// #define __ONEW__;

class Screen {
  int i;

 public:
  Screen(int x) : i(x) {}
  int get() { return i; }

#ifdef __ONEW__
  void* operator new(size_t);
  void operator delete(void* p, size_t);
#endif

 private:
  // 这种方式多耗用一个next
  Screen* next;
  static Screen* freeStore;
  static const int sceenChunk = 24;
};

Screen* Screen::freeStore = nullptr;

#ifdef  __ONEW__
void* Screen::operator new(size_t size) {
  std::cout << "void* Screen::operator new(size_t size)" << std::endl;
  if (freeStore == nullptr) {
    std::cout << "realloc " << std::endl;
    size_t chunk = sceenChunk * size;
    freeStore = (Screen*)(::operator new(chunk));
    Screen* cur = freeStore;
    for (size_t i = 0; i < sceenChunk - 1; i++) {
      cur->next = cur + 1;
      ++cur;
    }
    cur->next = nullptr;
  }
  void* result = freeStore;
  freeStore = freeStore->next;
  return result;
}

void Screen::operator delete(void* p, size_t){
  std::cout << "void Screen::operator delete(void* p, size_t)" << std::endl;
  Screen* __node = (Screen*)p;
  __node->next = freeStore;
  freeStore = __node;
}
#endif


int main(){
  Screen* ps = new Screen(1);
  delete ps;
  std::cout << sizeof(Screen) << std::endl;
  size_t N = 100;
  Screen* p[N];
  for(int i = 0; i < N; i++)
    p[i] = new Screen(1);
  for(int i = 0; i < 10; i++)
    std::cout << p[i] << std::endl;
  return 0;
}

// -exec x /10xg 0x55555556b2c0
// x examine 
// /nfu
// n 内存单元个数
// f 每内存单元显示方式，x d o 等
// u 一个内存单元长度 g八字节 w四字节



// 有cookie, 间隔32字节
// 0x55555556aeb0
// 0x55555556b2e0
// 0x55555556b300
// 0x55555556b320
// 0x55555556b340
// 0x55555556b360
// 0x55555556b380
// 0x55555556b3a0
// 0x55555556b3c0
// 0x55555556b3e0

// -exec p p
// $1 = {0x55555556aeb0, 0x55555556b2e0, 0x55555556b300, 0x55555556b320, 0x55555556b340, 0x55555556b360, .....}
// -exec x /10xg 0x55555556b2e0
// 0x55555556b2e0:	0x0000000000000001	0x0000000000000000
// 0x55555556b2f0:	0x0000000000000000	0x0000000000000021
// 0x55555556b300:	0x0000000000000001	0x0000000000000000
// 0x55555556b310:	0x0000000000000000	0x0000000000000021
// 0x55555556b320:	0x0000000000000001	0x0000000000000000
// 0x0000000000000021 2 * 16 = 32

// #define __ONEW__
// 无cookie 只有16字节 align((int + Screen*))
// 0x55555556b2c0
// 0x55555556b2d0
// 0x55555556b2e0
// 0x55555556b2f0
// 0x55555556b300
// 0x55555556b310
// 0x55555556b320
// 0x55555556b330
// 0x55555556b340
// 0x55555556b350

