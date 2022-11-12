#include <bits/stdc++.h>

// using namespace std;

void* myAlloc(size_t size) { return malloc(size); }

void myFree(void* ptr) { free(ptr); }

#if 0
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

class Bad{};

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
    // 抛出expection
    // 测试 
    throw Bad();
  }
  // virtual
  ~Foo() { std::cout << "dtor. this=" << this << " id=" << _id << std::endl; }

  // 必须是静态的，无法通过对象调用，隐式static
  static void* operator new(size_t size) {
    // Foo* p = (Foo*)::operator new(size);
    Foo* p = (Foo*)malloc(size);
    std::cout << "Foo::operator new() p = " << p << std::endl;
    return p;
  }
 
  // 标准库提供的placement new()的重载(形式)
  // new(void*)object;
  static void* operator new(size_t size, void* start){
    return start;
  }

  // 崭新的placement new
  // new(long)object;
  static void* operator new(size_t size, long extra){
    return malloc(size + extra);
  }
  // 'operator new' takes type size_t ('unsigned long') as first parameter
  // static void* operator new(long extra, char init){}

  static void* operator new(size_t size, long extra, char init){
    return malloc(size + extra);
  }

  static void* operator new[](size_t size) {
    Foo* p = (Foo*)malloc(size);
    std::cout << "Foo::operator new[]() p = " << p << std::endl;
    return p;
  }
   static void operator delete(void* p, size_t /*optional*/) noexcept {
    std::cout << "Foo::operator delete() p = " << p << std::endl;
    // free(p);
    // ::operator delete(p);
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
  
  // 这样调用(加上global scope operator::)会绕过所有 overloaded function，强迫使用 global
  // version
  {
    Foo* pf = ::new Foo;
    ::delete pf;
    Foo* pa = ::new Foo[5];
    ::delete[] pa;
  }

  return 0;
}
