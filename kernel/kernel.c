#include "hardware.h"
#include "segmentation.h"
#include "sched.h"
#include "interrupt.h"
#include "memory.h"
#include "util.h"

#pragma GCC diagnostic ignored "-Wunused-parameter" // For l6 (kernel_param_dummy)

void start(void *SystemTable __attribute__ ((unused)), struct HardwareInfo *_hardware_info, unsigned long long kernel_param_dummy) {
  // From here - Put this part at the top of start() function
  // Do not use _hardware_info directry since this data is located in UEFI-app space
  //my_hardware_info = *_hardware_info;
  hardware_info = *_hardware_info;
  init_segmentation();
  init_virtual_memory();
  init_intr();
  // To here - Put this part at the top of start() function

  // Delete me. I'm a sample code.
  //for (unsigned int i = 0; i < hardware_info.fb.height; i++) {
    //for (unsigned int j = 0; j < hardware_info.fb.width; j++) {
      //struct Pixel *pixel = my_hardware_info.fb.base + my_hardware_info.fb.width * i + j;
      // † AYAME †
      //pixel->r = 111;
      //pixel->g = 51;
      //pixel->b = 129;
    //}
  //}
  init_frame_buffer(_hardware_info);
  //puts("DEFGHI\nABC");
  //puth(31, 4);
  puth(kernel_param_dummy, 7);
  int data[8] = {
	0b01111100,
	0b10010010,
	0b10010010,
	0b11111111,
	0b00010010,
	0b10010010,
	0b01111100,
	0b00010000
  };
  // To here - sample code
 //for (int k = 0; k < 8; k++) { 
	 //for (int m = 0; m < 8; m++) {
		 //if (((data[k] >> (8-m)) & 1) == 1) {
			 //struct Pixel *pixel = hardware_info.fb.base + hardware_info.fb.width * (k) + m; 
			 //pixel->r = 255;
			 //pixel->g = 255;
			 //pixel->b = 255;
		 //}
	 //}
  //}

  // Do not delete it!
  while (1);
}
