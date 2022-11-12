#include <complex>
#include <iostream>
#include <string>

// 版本3, 专属于每个class的allocator
class allocator {
  // embeded pointer
  // 借用前8字节当作next指针
  struct obj {
    obj* next;
  };
  obj* my_freelist = nullptr;
  const int MAX_SIZE = 5;

 public:
  void* allocate(size_t size) {
    void* p;
    if (!my_freelist) {
      my_freelist = (obj*)malloc(size * MAX_SIZE);
      obj* cur = my_freelist;
      for (int i = 1; i < MAX_SIZE - 1; ++i) {
        cur->next = (obj*)((char*)cur + size);
        cur = cur->next;
      }
      cur->next = nullptr;
    }
    p = my_freelist;
    my_freelist = my_freelist->next;
    return p;
  }

  void deallocate(void* p, size_t) {
    ((obj*)p)->next = my_freelist;
    my_freelist = (obj*)p;
  }
};

class Foo {
  static allocator myAlloc;
  long i;
  char c;

 public:
  static void* operator new(size_t size) { return myAlloc.allocate(size); }
  static void operator delete(void* p, size_t size) {
    return myAlloc.deallocate(p, size);
  }
};

allocator Foo::myAlloc;

class Goo {
  static allocator myAlloc;
  std::complex<int> c;
  std::string s;
 public:
  static void* operator new(size_t size) { return myAlloc.allocate(size); }
  static void operator delete(void* p, size_t size) {
    return myAlloc.deallocate(p, size);
  }
};

allocator Goo::myAlloc;

int main() {
  Foo* p[10];
  for (int i = 0; i < 10; i++) p[i] = new Foo;
  for (int i = 0; i < 10; i++) std::cout << p[i] << std::endl;
  for (int i = 0; i < 10; i++) delete p[i];
  Goo* p1[10];
  for (int i = 0; i < 10; i++) p1[i] = new Goo;
  for (int i = 0; i < 10; i++) std::cout << p1[i] << std::endl;
  for (int i = 0; i < 10; i++) delete p1[i];
}