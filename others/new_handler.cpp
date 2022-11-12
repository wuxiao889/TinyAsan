#include <iostream>

using namespace std;

void myhandler(){
  std::cout << "out of memory\n";
  abort();
}

int main(){
  new_handler handler;
  set_new_handler(myhandler);
  int* p = (int*)::operator new(1e9);
  getchar();
}