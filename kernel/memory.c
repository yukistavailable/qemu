#include "memory.h"
#include "sched.h"

struct Entry {
  unsigned int present : 1;
  unsigned int writable : 1;
  unsigned int user_accessible : 1;
  unsigned int write_through_caching : 1;
  unsigned int disable_cache : 1;
  unsigned int accessed : 1;
  unsigned int dirty : 1;
  unsigned int huge_page : 1;
  unsigned int global : 1;
  unsigned int available1 : 3;
  unsigned long long physical_address : 40;
  unsigned int available2 : 11;
  unsigned int no_execute : 1;
} __attribute__((packed));

unsigned long long kernel_cr3;
unsigned long long task_cr3s[3];
struct Entry PML4s[3][512]__attribute__((aligned(4096)));
struct Entry PDPs[3][512]__attribute__((aligned(4096)));
struct Entry PDs[3][512]__attribute__((aligned(4096)));
struct Entry PTs[3][9][512]__attribute__((aligned(4096)));
struct Entry kernel_PD[512]__attribute__((aligned(4096)));
struct Entry kernel_PTs[9][512]__attribute__((aligned(4096)));
struct Entry io_PD[512]__attribute__((aligned(4096)));
struct Entry fb_PT[512]__attribute__((aligned(4096)));
struct Entry lapic_PT[512]__attribute__((aligned(4096)));
void init_virtual_memory() {
  // Save kernel cr3 register value
  asm volatile ("mov %%cr3, %0" : "=r"(kernel_cr3));
  // Page mapping
  //
  // Here 10
  //
  task_cr3s[0]=(unsigned long long)PML4s[0];
  task_cr3s[1]=(unsigned long long)PML4s[1];
  task_cr3s[2]=(unsigned long long)PML4s[2];
  for(int i=0;i<3;i++){
    PML4s[i][0].present=1;
    PML4s[i][0].writable=1;
    PML4s[i][0].physical_address=(unsigned long long)PDPs[i]>>12;
  }
  for(int i=0;i<3;i++){
    // 0x40000000 -> 2 ** 30; 31komega1
    PDPs[i][1].present=1;
    PDPs[i][1].writable=1;
    PDPs[i][1].physical_address=(unsigned long long)PDs[i]>>12;
    // 0x0C0000000 -> 3 * 2 ** 30; 29 and 30komega1
    PDPs[i][3].present=1;
    PDPs[i][3].writable=1;
    PDPs[i][3].physical_address=(unsigned long long)io_PD>>12;
    // 0x100000000 -> 2 ** 32+...; 33komega1
    PDPs[i][4].present=1;
    PDPs[i][4].writable=1;
    PDPs[i][4].physical_address=(unsigned long long)kernel_PD>>12;
  }
  for(int i=0;i<3;i++){
    for(int j=0;j<9;j++){
      PDs[i][j].present=1;
      PDs[i][j].writable=1;
      PDs[i][j].physical_address=(unsigned long long)PTs[i][j]>>12;
    }
  }
  for(int i=0;i<3;i++){
    for(int j=0;j<9;j++){
      for(int k=0;k<512;k++){
        PTs[i][j][k].present=1;
        PTs[i][j][k].writable=1;
        PTs[i][j][k].physical_address=0x104000+(i*0x1000)+(j<<9)+k;
      }
    }
  }
  io_PD[0].present=1;
  io_PD[0].writable=1;
  io_PD[0].physical_address=(unsigned long long)fb_PT>>12;
  io_PD[0b111110111].present=1;
  io_PD[0b111110111].writable=1;
  io_PD[0b111110111].physical_address=(unsigned long long)lapic_PT>>12;
  for(int i=0;i<512;i++){
    fb_PT[i].present=1;
    fb_PT[i].writable=1;
    fb_PT[i].physical_address=0x0c0000+i;
  }
  lapic_PT[0].present=1;
  lapic_PT[0].writable=1;
  lapic_PT[0].physical_address=0x0fee00;
  for(int i=0;i<9;i++){
    kernel_PD[i].present=1;
    kernel_PD[i].writable=1;
    kernel_PD[i].physical_address=(unsigned long long)kernel_PTs[i]>>12;
  }
  for(int i=0;i<9;i++){
    for(int j=0;j<512;j++){
      kernel_PTs[i][j].present=1;
      kernel_PTs[i][j].writable=1;
      kernel_PTs[i][j].physical_address=0x100000+(i<<9)+j;
    }
  }
}