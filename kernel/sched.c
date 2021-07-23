#pragma GCC diagnostic ignored "-Wunused-parameter"
#include "util.h"
#include "lapic_timer.h"
#include "memory.h"

volatile unsigned int *lapic_eoi2 = (unsigned int *)0xfee000b0;

struct Task {
  unsigned long long sp;
  unsigned long long cr3;
};

struct Task tasks[3];
unsigned int current_task;
unsigned long long rsp0 = 0x41000000;
unsigned long long rip0 = 0x40000000;
unsigned long long rsp1 = 0x41000000;
unsigned long long rip1 = 0x40000000;
unsigned long long rsp2 = 0x41000000;
unsigned long long rip2 = 0x40000000;

void init_tasks() {
    puts("\ninit tasks\n");
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
  unsigned long long task0_cr3 = task_cr3s[0];
  unsigned long long task1_cr3 = task_cr3s[1];
  unsigned long long task2_cr3 = task_cr3s[2];
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
puts("\n");
puts("register info");
puts("\n");
puth(kernel_cr3, 8);
puts("\n");
puth(task1_cr3, 8);
puts("\n");
puth(sp, 8);
puts("\n");
puth(rsp1, 8);
puts("\n");
  asm volatile(
    "mov %[rsp1], %%rsp\n"
    "mov %[cr3], %%cr3\n"
    "push %[lss1]\n"
    "push %[rsp1]\n"
    "pushfq\n"
    "push %[lcs1]\n"
    "push %[rip1]\n"
    "mov %[sp], %%rsp\n"
    "mov %[kcr3], %%cr3\n"
    ::[rsp1]"m"(rsp1), 
    [cr3]"r"(task1_cr3), 
    [lss1]"g"(lss1), 
    [lcs1]"g"(lcs1), 
    [rip1]"g"(rip1),
    [sp]"m"(sp),
    [kcr3]"r"(kernel_cr3)
  );
//   asm volatile(
//     "mov %[cr3], %%cr3\n"
//     ::[cr3]"a"(task1_cr3)
//   );
//   asm volatile(
//     "push %[lss1]\n"
//     "push %[rsp1]\n"
//     "pushfq\n"
//     "push %[lcs1]\n"
//     "push %[rip1]\n"
//     ::[lss1]"g"(lss1), [rsp1]"g"(rsp1), [lcs1]"g"(lcs1), [rip1]"g"(rip1)
//   );
//   asm volatile(
//       "mov %0, %%rsp\n"
//       "mov %1, %%cr3\n"
//       ::"m"(sp),
//         "a"(kernel_cr3));
//   asm volatile(
//     "mov %[rsp2], %%rsp\n"
//     ::[rsp2]"m"(rsp2)
//   );
//   asm volatile(
//     "mov %[cr3], %%cr3\n"
//     ::[cr3]"a"(task2_cr3)
//   );
//   asm volatile(
//     "push %[lss2]\n"
//     "push %[rsp2]\n"
//     "pushfq\n"
//     "push %[lcs2]\n"
//     "push %[rip2]\n"
//     ::[lss2]"g"(lss2), [rsp2]"g"(rsp2), [lcs2]"g"(lcs2), [rip2]"g"(rip2)
//   );
//   asm volatile("mov %0, %%rsp"::"m"(sp));
//   asm volatile(
//     "mov %[cr3], %%cr3\n"
//     ::[cr3]"a"(kernel_cr3)
//   );
  asm volatile(
    "mov %[rsp2], %%rsp\n"
    "mov %[cr3], %%cr3\n"
    "push %[lss2]\n"
    "push %[rsp2]\n"
    "pushfq\n"
    "push %[lcs2]\n"
    "push %[rip2]\n"
    "mov %[sp], %%rsp\n"
    "mov %[kernel_cr3], %%cr3\n"
    ::[rsp2]"m"(rsp2), 
    [cr3]"r"(task2_cr3), 
    [lss2]"g"(lss2), 
    [lcs2]"g"(lcs2), 
    [rip2]"g"(rip2),
    [sp]"m"(sp),
    [kernel_cr3]"r"(kernel_cr3)
  );
  arsp1 = rsp1 - 96;
  arsp2 = rsp2 - 96;
  puts("\narp2\n");
  puth(arsp2, 8);
//   asm volatile("mov %0, %%rsp"::"m"(sp));
  task0->sp = rsp0;
  task1->sp = arsp1;
  task2->sp = arsp2;
    puts("\n");
    puts("\n");
    puth(rsp0, 8);
    puts("\n");
    puth(task0_cr3, 8);
//   asm volatile(
//       "mov %0, %%rsp\n"
//       "mov %1, %%cr3\n"
//       ::"m"(rsp0),
//         "r"(task0_cr3)
//     );
//   while(1);
  puts("init tasks");
  asm volatile(
      "mov %0, %%rsp\n"
      "mov %1, %%cr3\n"
      "jmp *%2\n"
      ::"m"(rsp0),
        "r"(task0_cr3),
        "m"(rip0));
}
void schedule(unsigned long long sp) {
//   asm volatile(
//     "mov %[cr3], %%cr3\n"
//     ::[cr3]"a"(kernel_cr3)
//   );
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

  unsigned long long next_cr3;
  if (current_task==0) next_cr3=task_cr3s[0];
  else if (current_task==1) next_cr3=task_cr3s[1];
  else if (current_task==2) next_cr3=task_cr3s[2];
  puts("\nnew task\n");
  puth(current_task, 8);
  puts("\n");
  puth(next_cr3, 8);

// switch sp
  newsp = newtask->sp;
  puth(newsp, 8);
  *lapic_eoi2 = 0;
//   if (current_task == 0) {
//       while(1);
//   }
//   asm volatile("mov %0, %%rsp"::"m"(newsp));
//   asm volatile(
//     "mov %[cr3], %%cr3\n"
//     ::[cr3]"a"(next_cr3)
//   );

  asm volatile(
    "mov %0, %%rsp\n"
    "mov %1, %%cr3\n"
    "pop %%rbp\n"
    "pop %%rsi\n"
    "pop %%rdi\n"
    "pop %%rdx\n"
    "pop %%rcx\n"
    "pop %%rbx\n"
    "pop %%rax\n"
    "iretq\n"
    "jmp *%2"
  ::"m"(newsp),
  "r"(next_cr3),
  "m"(rip));
  // while(1);

// jump
  // *lapic_eoi2 = 0;
  // *lapic_eoi = 0;
//   asm volatile("jmp *%0"::"m"(rip));
}