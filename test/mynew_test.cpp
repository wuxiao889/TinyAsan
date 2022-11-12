#include <stdio.h>
#include <new>
#include <cstdlib>

void *operator new(size_t size) {
  printf("mynew\n");
  return malloc(size);
}

// -fsanitizer
void operator delete(void *p) noexcept{
  printf("mydelete\n");
  free(p);
}

int main() {
  int *p = new int(1);
  delete p;
  return 0;
}