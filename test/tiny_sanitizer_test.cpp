#include "tiny_sanitizer.hpp"
#include <string>
#include <vector>
#include <stdlib.h>

using namespace std;

// not detected
char* LeekWithMalloc(){
  char* p = static_cast<char*>(malloc(1024));
  return p;
}

string* LeekWithNew(){
  string* p = new string("nihao");
  return p;
}

int main(){
  int* p = new int();
  delete p;

  int* p2 = new int();

  vector<int> nums;
  nums.reserve(10);

  LeekWithNew();

  LeekWithMalloc();
}