#include "hardware.h"
#include "segmentation.h"
#include "sched.h"
#include "interrupt.h"
#include "memory.h"
#include "util.h"
#include "pm_timer.h"
#include "lapic_timer.h"
#include "syscall.h"
#include "memory.h"
#include "virtio.h"

#pragma GCC diagnostic ignored "-Wunused-parameter" // For l6 (kernel_param_dummy)
void schedule(unsigned long long sp);

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info, unsigned long long kernel_param_dummy) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  //my_hardware_info = *_hardware_info;
  hardware_info = *_hardware_info;
  init_segmentation();
  init_virtual_memory();
  init_frame_buffer(_hardware_info);
  init_intr();
  init_virtual_memory();
  // To here - Put this part at the top of start() function

  // 8
  // puts("start");
  // init_acpi_pm_timer(hardware_info.rsdp);
  // pm_timer_wait_millisec(1000);
  // // unsigned int fre = measure_lapic_freq_khz();
  // // puth(fre, 8);
  // puts("Set CallBack");
  // void * schedule_address;
  // asm volatile ("lea schedule(%%rip), %0" : "=r"(schedule_address));
  // puts("\nstart\n");
  // lapic_periodic_exec(1000, (void *) schedule_address);
  // puts("\nfinish\n");
  // puth(kernel_param_dummy, 7);

  // 9

  // Do not delete it!
  char s1[10] = "FIRST";
  char s2[10] = "SECOND";
  char * sp1;
  char * sp2;
  unsigned long long id;
  id = 1;
  unsigned long long idp = (unsigned long long) &id;
  sp1 = s1;
  sp2 = s2;
  unsigned long long syscall_ret;
  puts("\nSTART\n");
  
  //9-A
  // asm volatile (
  //   "movq %[id], %%rax\n"
  //   "movq %[id], %%rdi\n"
  //   "movq %[arg1], %%rsi\n"
  //   "int $0x80\n"
  //   "movq %%rax, %[ret]\n"
  //   :[ret]"=r"(syscall_ret)
  //   :[id]"g"(id),
  //   [arg1]"m"((unsigned long long) sp1)
  // );
  // puts("\n");
  // asm volatile (
  //   "movq %[id], %%rax\n"
  //   "movq %[id], %%rdi\n"
  //   "movq %[arg1], %%rsi\n"
  //   "int $0x80\n"
  //   "movq %%rax, %[ret]\n"
  //   :[ret]"=r"(syscall_ret)
  //   :[id]"g"(id),
  //   [arg1]"m"((unsigned long long) sp2)
  // );

  //9-B
  // void * schedule_address;
  // asm volatile ("lea schedule(%%rip), %0" : "=r"(schedule_address));
  // lapic_periodic_exec(1000, (void *) schedule_address);
  // // while(1);
  // init_tasks();
  find();
  while (1);
}
