#include "util.h"

unsigned long long syscall_puts(char* str) {
  // while (1);
  puts(str);
  return 0;
}

unsigned long long syscall_handler_internal(unsigned long long syscall_id,
    unsigned long long arg1, unsigned long long arg2, unsigned long long arg3) {
  unsigned long long ret;
  // puts("\nsyscall_handler_start\n");
  // puth(syscall_id, 8);
  // puts("\n");
  // puth(arg1, 8);
  // while (1);
  switch (syscall_id) {
    case 0x1:
  // puts("\ncase: 0x1\n");
  // while (1);
      ret = syscall_puts((char *)arg1);
      // ret = syscall_puts("START");
      // while (1);
      break;
    default:
      break;
  }
  // puth(ret, 8);

  return ret;
}
