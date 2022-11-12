// stacktrace.h (c) 2008, Timo Bingmann from http://idlebox.net/
// published under the WTFPL v2.0

#ifndef _STACKTRACE_H_
#define _STACKTRACE_H_

#include <cxxabi.h>
#include <execinfo.h>
#include <stdio.h>
#include <stdlib.h>

#define BT_BUF_SIZE 100

/** Print a demangled stack backtrace of the caller function to FILE* out. */
static inline char* print_stacktrace(FILE* out = stderr,
                                     unsigned int max_frames = 63) {
  char* buf = (char*)malloc(1024);
  char* cur = buf;
  // cur += snprintf(cur, 1024, "stack trace:\n");
  // fprintf(out, "stack trace:\n");

  // storage array for stack trace address data
  void* addrlist[max_frames + 1];

  // retrieve current stack addresses
  int addrlen = backtrace(addrlist, sizeof(addrlist) / sizeof(void*));

  if (addrlen == 0) {
    cur += snprintf(cur, 1024, "  <empty, possibly corrupt>\n");
    // fprintf(out, "  <empty, possibly corrupt>\n");
    return buf;
  }

  // resolve addresses into strings containing "filename(function+address)",
  // this array must be free()-ed
  char** symbollist = backtrace_symbols(addrlist, addrlen);

  // allocate string which will be filled with the demangled function name
  size_t funcnamesize = 256;
  char* funcname = (char*)malloc(funcnamesize);

  // iterate over the returned symbol lines. skip the first, it is the
  // address of this function.
  for (int i = 1; i < addrlen; i++) {
    char *begin_name = 0, *begin_offset = 0, *end_offset = 0;

    // find parentheses and +address offset surrounding the mangled name:
    // ./module(function+0x15c) [0x8048a6d]
    for (char* p = symbollist[i]; *p; ++p) {
      if (*p == '(')
        begin_name = p;
      else if (*p == '+')
        begin_offset = p;
      else if (*p == ')' && begin_offset) {
        end_offset = p;
        break;
      }
    }

    if (begin_name && begin_offset && end_offset && begin_name < begin_offset) {
      *begin_name++ = '\0';
      *begin_offset++ = '\0';
      *end_offset = '\0';

      // mangled name is now in [begin_name, begin_offset) and caller
      // offset in [begin_offset, end_offset). now apply
      // __cxa_demangle():

      int status;
      char* ret =
          abi::__cxa_demangle(begin_name, funcname, &funcnamesize, &status);
      if (status == 0) {
        funcname = ret;  // use possibly realloc()-ed string
        cur += snprintf(cur, 1024, "  %s : %s+%s\n", symbollist[i], funcname,
                        begin_offset);
        // fprintf(out, "  %s : %s+%s\n", symbollist[i], funcname,
        // begin_offset);
      } else {
        // demangling failed. Output function name as a C function with
        // no arguments.
        cur += snprintf(cur, 1024, "  %s : %s()+%s\n", symbollist[i],
                        begin_name, begin_offset);
        // fprintf(out, "  %s : %s()+%s\n", symbollist[i], begin_name,
        //         begin_offset);
      }
    } else {
      // couldn't parse the line? print the whole line.
      cur += snprintf(cur, 1024, "  %s\n", symbollist[i]);
      // fprintf(out, "  %s\n", symbollist[i]);
    }
  }

  free(funcname);
  free(symbollist);
  // printf("size %ld\n", cur- buf);
  return buf;
}

inline
char* mybacktrace(void) {
  int nptrs;
  void* buffer[BT_BUF_SIZE];
  char** strings;

  nptrs = backtrace(buffer, BT_BUF_SIZE);
  printf("backtrace() returned %d addresses\n", nptrs);

  /* The call backtrace_symbols_fd(buffer, nptrs, STDOUT_FILENO)
     would produce similar output to the following: */

  strings = backtrace_symbols(buffer, nptrs);
  if (strings == NULL) {
    perror("backtrace_symbols");
    exit(EXIT_FAILURE);
  }
  char* res = (char*)malloc(1024);
  char* cur = res;
  for (int j = 2; j < nptrs; j++){
    cur += snprintf(cur, 1024,"%s\n", strings[j]);
  } 
  // for (int j = 0; j < nptrs; j++) printf("%s\n", strings[j]);

  free(strings);
  return res;
}

#endif  // _STACKTRACE_H_