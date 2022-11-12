#ifndef __M_TINYSANITIZER_H
#define __M_TINYSANITIZER_H

#include <stdio.h>

#include <ext/malloc_allocator.h>
#include <unordered_map>

#include <stacktrace.h>

namespace TASAN {

class TinySanitizer {
  
  TinySanitizer() = default;

 public:
 
  using pair_type = std::pair<char*, size_t>;

  TinySanitizer(const TinySanitizer&) = delete;

  TinySanitizer& operator=(const TinySanitizer&) = delete;

  ~TinySanitizer();

  void insert(void* ptr, char* trace, size_t size);

  void erase(void* ptr);

  static TinySanitizer instance;

 private:
  std::unordered_map<void*, pair_type, std::hash<void*>, std::equal_to<void*>,
                     __gnu_cxx::malloc_allocator<std::pair<void* const, pair_type>>>  map_;
};

}  // namespace TASAN

#endif  // !__M_TINYSANITIZER_H
