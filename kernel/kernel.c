#include "hardware.h"
#include "segmentation.h"
#include "sched.h"
#include "interrupt.h"
#include "memory.h"
#include "util.h"
#include "pm_timer.h"
#include "pm_timer.h"
#include "lapic_timer.h"

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
  // To here - Put this part at the top of start() function

  puts("start");
  init_acpi_pm_timer(hardware_info.rsdp);
  pm_timer_wait_millisec(1000);
  // unsigned int fre = measure_lapic_freq_khz();
  // puth(fre, 8);
  puts("Set CallBack");
  void * schedule_address;
  asm volatile ("lea schedule(%%rip), %0" : "=r"(schedule_address));
  puts("\nstart\n");
  lapic_periodic_exec(1000, (void *) schedule_address);
  puts("\nfinish\n");
  // puth(kernel_param_dummy, 7);
  // Do not delete it!
  while (1);
}
