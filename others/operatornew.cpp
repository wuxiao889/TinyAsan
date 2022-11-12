#include <bits/stdc++.h>

// using namespace std;

#if 1

void* myAlloc(size_t size) { return malloc(size); }

void myFree(void* ptr) { free(ptr); }

// 重载全局new delete 不可以在一个命名空间中
// 影响很大，一般不重载
void* operator new(size_t size) {
  std::cout << "global new()\n";
  return myAlloc(size);
}

void* operator new[](size_t size) {
  std::cout << "global new[]()\n";
  return myAlloc(size);
}

void operator delete(void* ptr) noexcept{
  std::cout << "global delete()\n";
  return myFree(ptr);
}

void operator delete[](void* ptr) noexcept{
  std::cout << "global delete[]()\n";
  return myFree(ptr);
}
#endif

class Foo {
  int _id;
  long _data;
  std::string _str;

 public:
  Foo() : _id(0) {
    std::cout << "default ctor. this=" << this << " id=" << _id << std::endl;
  }
  Foo(int i) : _id(i) {
    std::cout << "ctor. this=" << this << " id=" << _id << std::endl;
  }
  virtual
  ~Foo() { std::cout << "dtor. this=" << this << " id=" << _id << std::endl; }

  // 必须是静态的，无法通过对象调用，隐式static
  static void* operator new(size_t size) {
    // Foo* p = (Foo*)::operator new(size);
    Foo* p = (Foo*)malloc(size);
    std::cout << "Foo::operator new() p = " << p << std::endl;
    return p;
  }
  static void operator delete(void* p, size_t /*optional*/) noexcept {
    std::cout << "Foo::operator delete() p = " << p << std::endl;
    free(p);
    // ::operator delete(p);
  }
  static void* operator new[](size_t size) {
    Foo* p = (Foo*)malloc(size);
    std::cout << "Foo::operator new[]() p = " << p << std::endl;
    return p;
  }
  static void operator delete[](void* p, size_t /*optional*/) noexcept {
    std::cout << "Foo::operator delete[]() p = " << p << std::endl;
    free(p);
  }
};



int main() {
  // non-virtual dtor 48
  // virtual dtor 56
  std::cout << "sizeof(Foo) " << sizeof(Foo) << std::endl;
  std::cout << "-------" << std::endl;

  {
    // 若无members就调用globals
    Foo* pf = new Foo(7);
    delete pf;
    Foo* pa = new Foo[5];
    delete[] pa;
  }
  std::cout << "-------" << std::endl;

  // 这样调用(加上global scope operator::)会绕过所有 overloaded
  // function，强迫使用 global version
  {
    Foo* pf = ::new Foo;
    ::delete pf;
    Foo* pa = ::new Foo[5];
    ::delete[] pa;
  }

  return 0;
}

// sizeof(Foo) 48
// -------
// Foo::operator new() p = 0x55555556b2c0
// ctor. this=0x55555556b2c0 id=7
// dtor. this=0x55555556b2c0 id=7
// Foo::operator delete() p = 0x55555556b2c0
// Foo::operator new[]() p = 0x55555556b300
// default ctor. this=0x55555556b308 id=0
// default ctor. this=0x55555556b338 id=0
// default ctor. this=0x55555556b368 id=0
// default ctor. this=0x55555556b398 id=0
// default ctor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b398 id=0
// dtor. this=0x55555556b368 id=0
// dtor. this=0x55555556b338 id=0
// dtor. this=0x55555556b308 id=0
// Foo::operator delete[]() p = 0x55555556b300
// -------
// default ctor. this=0x55555556b2c0 id=0
// dtor. this=0x55555556b2c0 id=0
// default ctor. this=0x55555556b308 id=0
// default ctor. this=0x55555556b338 id=0
// default ctor. this=0x55555556b368 id=0
// default ctor. this=0x55555556b398 id=0
// default ctor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b398 id=0
// dtor. this=0x55555556b368 id=0
// dtor. this=0x55555556b338 id=0
// dtor. this=0x55555556b308 id=0

// sizeof(Foo) 48
// -------
// Foo::operator new() p = 0x55555556b2c0
// ctor. this=0x55555556b2c0 id=7
// dtor. this=0x55555556b2c0 id=7
// Foo::operator delete() p = 0x55555556b2c0
// Foo::operator new[]() p = 0x55555556b300
// default ctor. this=0x55555556b308 id=0
// default ctor. this=0x55555556b338 id=0
// default ctor. this=0x55555556b368 id=0
// default ctor. this=0x55555556b398 id=0
// default ctor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b398 id=0
// dtor. this=0x55555556b368 id=0
// dtor. this=0x55555556b338 id=0
// dtor. this=0x55555556b308 id=0
// Foo::operator delete[]() p = 0x55555556b300
// -------
// global new()
// default ctor. this=0x55555556b2c0 id=0
// dtor. this=0x55555556b2c0 id=0
// global delete()
// global new[]()
// default ctor. this=0x55555556b308 id=0
// default ctor. this=0x55555556b338 id=0
// default ctor. this=0x55555556b368 id=0
// default ctor. this=0x55555556b398 id=0
// default ctor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b3c8 id=0
// dtor. this=0x55555556b398 id=0
// dtor. this=0x55555556b368 id=0
// dtor. this=0x55555556b338 id=0
// dtor. this=0x55555556b308 id=0
// global delete[]()