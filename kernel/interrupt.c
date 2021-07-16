#include "interrupt.h"
#include "lapic_timer.h"
#include "util.h"
#include "syscall.h"

struct InterruptDescriptor {
  unsigned short offset_lo;
  unsigned short segment;
  unsigned short attribute;
  unsigned short offset_mid;
  unsigned int offset_hi;
  unsigned int reserved;
} __attribute__((packed));

struct InterruptDescriptor IDT[256];

void lapic_intr_handler();

void syscall_handler();

static void load_idt_to_idtr() {
  // Set idtr
  //
  // Here (8-B)
  //
  unsigned char data[10];
  unsigned long long idt_addr = (unsigned long long) IDT;
  unsigned int table_limit = 256 * 16 - 1;
  data[0] = table_limit & 0xFF;
  data[1] = table_limit >> 8 & 0xFF;
  data[2] = (idt_addr) & 0xFF;
  data[3] = (idt_addr >> 8) & 0xFF;
  data[4] = (idt_addr >> 16) & 0xFF;
  data[5] = (idt_addr >> 24) & 0xFF;
  data[6] = (idt_addr >> 32) & 0xFF;
  data[7] = (idt_addr >> 40) & 0xFF;
  data[8] = (idt_addr >> 48) & 0xFF;
  data[9] = (idt_addr >> 56) & 0xFF;
  asm volatile ("lidt %0;" :: "m"(data));
}

void init_intr() {
  puts("init_intr start");
  unsigned short cs;
  asm volatile ("mov %%cs, %0" : "=r"(cs));

  // void* lapic_intr_handler_addr;
  unsigned long long lapic_intr_handler_addr;
  asm volatile ("lea lapic_intr_handler(%%rip), %0" : "=r"(lapic_intr_handler_addr));

  unsigned long long syscall_handler_addr;
  // asm volatile ("lea syscall_handler(%%rip), %[handler]" : [handler]"=r"(syscall_handler_addr));
  asm volatile ("lea syscall_handler(%%rip), %0" : "=r"(syscall_handler_addr));

  // Register Local APIC handler
  //
  // Here (8-B)
  //
  // zero clear
  for (int i = 0; i < 256; i++) {
    struct InterruptDescriptor *idt = &(IDT[i]);
    idt->offset_lo = 0;
    idt->segment = 0;
    idt->attribute = 0;
    idt->offset_mid = 0;
    idt->offset_hi = 0;
    idt->reserved = 0;
  }
  struct InterruptDescriptor *idt = &(IDT[32]);
  idt->segment = cs;
  idt->attribute = 0b1000111000000000;
  idt->offset_hi = (lapic_intr_handler_addr >> 32) & 0xFFFFFFFF;
  idt->offset_mid = (lapic_intr_handler_addr >> 16) & 0xFFFF;
  idt->offset_lo = (lapic_intr_handler_addr) & 0xFFFF;
  idt->reserved = 1;

  struct InterruptDescriptor *idt80 = &(IDT[128]);
  idt80->segment = cs;
  idt80->attribute = 0b1000111000000000;
  idt80->offset_hi = (syscall_handler_addr >> 32) & 0xFFFFFFFF;
  idt80->offset_mid = (syscall_handler_addr >> 16) & 0xFFFF;
  idt80->offset_lo = (syscall_handler_addr) & 0xFFFF;
  idt80->reserved = 1;

  // Register Sycall handler
  //
  // Here (9-A)
  //

  // Tell CPU the location of IDT
  load_idt_to_idtr();

  // Set IF bit in RFLAGS register
  asm volatile ("sti");
}
