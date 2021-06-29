#include "hardware.h"
#include "util.h"

// const unsigned int freq_hz = 3579545;
// unsigned short pm_timer_blk;
// char pm_timer_is_32;
void init_acpi_pm_timer(struct RSDP *rsdp);
void pm_timer_wait_millisec(unsigned int msec);