#include "lapic_timer.h"
#include "util.h"
#include "pm_timer.h"

#define COUNT_MAX 0xffffffff

volatile unsigned int *lvt_timer = (unsigned int *)0xfee00320;
volatile unsigned int *initial_count = (unsigned int *)0xfee00380;
volatile unsigned int *current_count = (unsigned int *)0xfee00390;
volatile unsigned int *divide_config = (unsigned int *)0xfee003e0;

unsigned int lapic_timer_freq_khz = 1011577;

volatile unsigned int *lapic_eoi = (unsigned int *)0xfee000b0;


void (*reserved_callback)(unsigned long long);
// void (*reserved_callback)();

unsigned int measure_lapic_freq_khz() {
  // Initialize LAPIC One-Shot timer
  // 13~15bit no change
  unsigned int tmp = *lvt_timer & 0b1110000000000000;
  *lvt_timer = 0b0010000000000000000 + tmp;
  // Mask == 1 -> not interrupt
  *initial_count = 100000000;

  // Wait 1000ms
  pm_timer_wait_millisec(10);

  // Calculate LAPIC Freq.
  lapic_timer_freq_khz = (*initial_count - *current_count) / 10;
  // puts("\ncurrent_count\n");
  // puth(*current_count, 8);
  // puts("\nlapic_timer_freq_khz\n");
  // puth(lapic_timer_freq_khz, 8);
  // puts("\n\n");

  return lapic_timer_freq_khz;
}

void lapic_periodic_exec(unsigned int msec, void *callback) {
  if (!lapic_timer_freq_khz) {
    puts("Call measure_lapic_freq_khz() beforehand\r\n");
    return;
  }
  puts("lapic_freq_khz");
  puth(lapic_timer_freq_khz, 8);
  // while (1) {

  // }
  reserved_callback = callback;
  puth((unsigned long long) reserved_callback, 8);
  // while (1) {

  // }
  unsigned int tmp = *lvt_timer & 0b1110000000000000;
  puth(tmp, 8);
  // *lvt_timer = 0b0010000000000000000 + tmp;
  *lvt_timer = 0b0100000000000100000 + tmp;
  // while (1) {

  // }
  //Vector == 32;
  *initial_count = msec*lapic_timer_freq_khz;
  // while (1) {
  // }
  puts("initial_count\n");
  puth(*initial_count, 8);


}

void lapic_intr_handler_internal(unsigned long long sp) {
  // Set End of Interrupt
  reserved_callback(sp);
  *lapic_eoi = 0;
}

// void lapic_intr_handler_internal() {
//   // Set End of Interrupt
//   // reserved_callback();
//   *lapic_eoi = 0;
// }