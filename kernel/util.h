#include "hardware.h"

void init_frame_buffer(struct HardwareInfo *hardware);
void putc(char c);
void puts(char *str);
void puth(unsigned long long value, unsigned char digits_len);
static int base;
static unsigned int fb_y;
static unsigned int fb_x;
static struct HardwareInfo my_hardware_info;
