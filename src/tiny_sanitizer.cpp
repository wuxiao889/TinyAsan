#include <tiny_sanitizer.hpp>

using namespace TASAN;

static size_t cnt_new;
static size_t cnt_delete;
static size_t total_new;

TinySanitizer TinySanitizer::instance;

TinySanitizer::~TinySanitizer() {
  if (map_.empty()) return;

  int cnt_leak = 0;
  size_t leak_size = 0;
  printf("=================================================================\n");
  printf("\033[1;31m\tERROR: TinySanitizer: detected memory leaks\033[0m\n\n");

  for (auto& entry : map_) {
    void* ptr = entry.first;
    pair_type ptr_data = entry.second;
    printf("\033[1;35mDirect leak of %ld byte(s) in 1 object(s) at %p allocated ""from:\033[0m\n",ptr_data.second, ptr);
    printf("%s\n", ptr_data.first);
    free(ptr_data.first);
    ++cnt_leak;
    leak_size += ptr_data.second;
  }
  printf("SUMMARY: AddressSanitizer: %ld byte(s) leaked in %d allocation(s).\n",leak_size, cnt_leak);
  printf("         total new/delete usage: %ld news, %ld deletes, %ld bytes allocated\n",cnt_new, cnt_delete, total_new);
}

void TinySanitizer::insert(void* ptr, char* trace, size_t size) {
  map_[ptr] = std::make_pair(trace, size);
}

void TinySanitizer::erase(void* ptr) {
  // 先free对应的堆栈记录
  free(map_[ptr].first);
  map_.erase(ptr);
}

void* operator new(size_t size) {
  void* ptr = malloc(size);
  // printf("my new %p\n", ptr);
  TinySanitizer::instance.insert(ptr, print_stacktrace(),size);
  ++cnt_new;
  total_new += size;
  return ptr;
}

void operator delete(void* ptr) {
  TinySanitizer::instance.erase(ptr);
  // printf("my delete %p\n", ptr);
  free(ptr);
  ++cnt_delete;
}