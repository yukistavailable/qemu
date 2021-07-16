#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "util.h"
#include "lapic_timer.h"
volatile unsigned int *lapic_eoi2 = (unsigned int *)0xfee000b0;
struct Task {
  unsigned long long sp;
};
struct Task tasks[3];
unsigned int current_task;
unsigned long long rsp0 = 0x105000000;
unsigned long long rip0 = 0x104000000;
unsigned long long rip1 = 0x105000000;
unsigned long long rsp1 = 0x106000000;
unsigned long long rip2 = 0x106000000;
unsigned long long rsp2 = 0x107000000;

void init_tasks() {
  current_task = 0;
  struct Task * task0 = &tasks[0];
  struct Task * task1 = &tasks[1];
  struct Task * task2 = &tasks[2];
  // init task1, task2;
  unsigned long long sp;
  unsigned long long arsp1;
  unsigned short ss1;
  unsigned short cs1;
  unsigned long long lss1;
  unsigned long long lcs1;
  unsigned long long arsp2;
  unsigned short ss2;
  unsigned short cs2;
  unsigned long long lss2;
  unsigned long long lcs2;
  asm volatile ("mov %%ss, %0":"=r"(ss1));
  asm volatile ("mov %%cs, %0":"=r"(cs1));
  asm volatile ("mov %%ss, %0":"=r"(ss2));
  asm volatile ("mov %%cs, %0":"=r"(cs2));
  lss1 = (unsigned long long) ss1;
  lcs1 = (unsigned long long) cs1;
  lss2 = (unsigned long long) ss2;
  lcs2 = (unsigned long long) cs2;
  puts("\nsp1\n");
  puth(rsp1, 8);
  asm volatile(
    "mov %%rsp, %0\n"
    :"=r"(sp)
  );
  asm volatile(
    "mov %[rsp1], %%rsp\n"
    ::[rsp1]"m"(rsp1)
  );
  asm volatile(
    "push %[lss1]\n"
    "push %[rsp1]\n"
    "pushfq\n"
    "push %[lcs1]\n"
    "push %[rip1]\n"
    ::[lss1]"g"(lss1), [rsp1]"g"(rsp1), [lcs1]"g"(lcs1), [rip1]"g"(rip1)
  );
  arsp1 = rsp1 - 96;
  // asm volatile (
  //   "mov %%rsp, %[sp]\n"
  //   "mov %[rsp1], %%rsp\n"
  //   "mov %[ss1], %%rax\n"
  //   "push %%rax\n"
  //   "mov %[rsp1], %%rax\n"
  //   "push %%rax\n"
  //   "pushfq\n"
  //   "mov %[cs1], %%rax\n"
  //   "push %%rax\n"
  //   "mov %[rip1], %%rax\n"
  //   "push %%rax\n"
  //   "push %%rax\n"
  //   "push %%rbx\n"
  //   "push %%rcx\n"
  //   "push %%rdx\n"
  //   "push %%rdi\n"
  //   "push %%rsi\n"
  //   "push %%rbp\n"
  //   "mov %%rsp, %[arsp1]\n"
  //   :[sp]"=r"(sp), [arsp1]"=r"(arsp1)
  //   :[ss1]"g"((unsigned long long) ss1),
  //   [rsp1]"m"(rsp1),
  //   [cs1]"g"((unsigned long long) cs1),
  //   [rip1]"m"(rip1)
  // );
  puts("\narp1\n");
  puth(arsp1, 8);
  puts("\ndiff\n");
  puth(rsp1 - arsp1, 8);
  // asm volatile (
  //   "mov %[rsp2], %%rsp\n"
  //   "mov %[ss2], %%rax\n"
  //   "push %%rax\n"
  //   "mov %[rsp2], %%rax\n"
  //   "push %%rax\n"
  //   "pushfq\n"
  //   "mov %[cs2], %%rax\n"
  //   "push %%rax\n"
  //   "mov %[rip2], %%rax\n"
  //   "push %%rax\n"
  //   "push %%rax\n"
  //   "push %%rbx\n"
  //   "push %%rcx\n"
  //   "push %%rdx\n"
  //   "push %%rdi\n"
  //   "push %%rsi\n"
  //   "push %%rbp\n"
  //   "mov %%rsp, %[arsp2]\n"
  //   :[arsp2]"=r"(arsp2)
  //   :[ss2]"g"((unsigned long long) ss2), 
  //   [rsp2]"m"(rsp2),
  //   [cs2]"g"((unsigned long long)cs2),
  //   [rip2]"m"(rip2)
  // );
  asm volatile(
    "mov %[rsp2], %%rsp\n"
    ::[rsp2]"m"(rsp2)
  );
  asm volatile(
    "push %[lss2]\n"
    "push %[rsp2]\n"
    "pushfq\n"
    "push %[lcs2]\n"
    "push %[rip2]\n"
    ::[lss2]"g"(lss2), [rsp2]"g"(rsp2), [lcs2]"g"(lcs2), [rip2]"g"(rip2)
  );
  arsp2 = rsp2 - 96;
  puts("\narp2\n");
  puth(arsp2, 8);
  asm volatile("mov %0, %%rsp"::"m"(sp));
  task0->sp = rsp0;
  task1->sp = arsp1;
  task2->sp = arsp2;
  puts("init tasks");
  asm volatile("mov %0, %%rsp"::"m"(rsp0));
  asm volatile("jmp *%0"::"m"(rip0));
}

void schedule(unsigned long long sp) {
  puts("\nschedule() called\r\n");
  puth(sp, 8);
  // unsigned long long nowsp;
  unsigned long long newsp;
  // sp wo hozon
  // asm volatile("mov %%rsp, %0":"=r"(nowsp));
  struct Task * task = &tasks[current_task];
  task->sp = sp;

  // switch task
  if (current_task==0) current_task=1;
  else if (current_task==1) current_task=2;
  else if (current_task==2) current_task=0;
  struct Task * newtask = &tasks[current_task];
  unsigned long long rip;
  if (current_task==0) rip=rip0;
  else if (current_task==1) rip=rip1;
  else if (current_task==2) rip=rip2;
  puts("\nnew task\n");
  puth(current_task, 8);

// switch sp
  newsp = newtask->sp;
  puth(newsp, 8);
  *lapic_eoi2 = 0;
  asm volatile("mov %0, %%rsp"::"m"(newsp));
  // while(1);

// hukki
  asm volatile(
    "pop %%rbp\n"
    "pop %%rsi\n"
    "pop %%rdi\n"
    "pop %%rdx\n"
    "pop %%rcx\n"
    "pop %%rbx\n"
    "pop %%rax\n"
    "iretq\n"
  ::);
  // while(1);

// jump
  // *lapic_eoi2 = 0;
  // *lapic_eoi = 0;
  asm volatile("jmp *%0"::"m"(rip));
}
