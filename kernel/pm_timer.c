#include "hardware.h"
#include "util.h"

const unsigned int freq_hz = 3579545;
unsigned short pm_timer_blk;
char pm_timer_is_32;

void init_acpi_pm_timer(struct RSDP *rsdp) {
    unsigned long long xsdt_address = rsdp->xsdt_address; 
    unsigned long long xsdt_address_add = xsdt_address + 1; 
    struct XSDT *xsdt = (struct XSDT *) xsdt_address;
    struct FADT ** fadt;
    unsigned long long fadt_address;
    unsigned int xsdt_length = (xsdt->sdth).length;
    // puts((xsdt->sdth).signature);
    // puts((char *) xsdt_address);
    // puth(xsdt_length, 5);
    for (unsigned int i = 0; i < xsdt_length - 36; i=i+8) {
        // puth(i, 5);
        unsigned long long sdth_address = xsdt_address + 36 + i;
        // *p = (struct FADT **) ((unsigned long long) xsdt_address + 36 + i);
        struct SDTH **tmp = (struct SDTH **) ((unsigned long long) xsdt_address + 36 + i);

        // struct SDTH *sdth = (unsigned long long) (xsdt_address + 36 + i);
        // struct SDTH *sdth = sdth_address;
        // puts(sdth->signature);
        // int c = compare_str("FACP", (char *) sdth_address, 4);
        // int c = compare_str("FACP", (char *) * ((struct FADT**) p), 4);
        // int c = compare_str("FACP", (*tmp)->signature, 4);
        // puth(c, 2);
        if (compare_str("FACP", (*tmp)->signature, 4) == 1) {
            fadt_address = xsdt_address + 36 + i;
            fadt = (struct FADT **) ((unsigned long long) xsdt_address + 36 + i);
            break;
        }
    }
    // pm_timer_blk = ((unsigned long long) *fadt) + 76;
    pm_timer_blk = (*fadt)->PM_TMR_BLK;
    if (((*fadt)->flags) >> 7 & 1) {
        puts("32\n");
        pm_timer_is_32 = 't';
    }
    else {
        puts("16\n");
        pm_timer_is_32 = 'f';
    }
}

void pm_timer_wait_millisec(unsigned int msec) {
    unsigned long long max = 0b11111111111111111111111111111111;
    unsigned long long tmp = freq_hz * msec / 1000;
    unsigned long long total_count = tmp % max;
    unsigned int start;
    unsigned int result;
    asm volatile("in %1, %0;"
    :"=a"(start)
    :"Nd"(pm_timer_blk));
    // puth(start, 8);
    // puts("\n");
    if (pm_timer_is_32 == 't') {
        while (1) {
            asm volatile("in %1, %0;"
            :"=a"(result)
            :"Nd"(pm_timer_blk));
            if (result - start >= total_count) break;
        }
        puts("\ntotal_count\n");
        puth(total_count, 8);
        puts("\ncount\n");
        puth(result - start, 8);
    }
    else if (pm_timer_is_32 == 'f') {
        while (1) {
            asm volatile("in %1, %0;"
            :"=a"(result)
            :"Nd"(pm_timer_blk));
            result = result & 0xFFFFFF; //24bit
            if (result - start >= total_count) break;
        }
    }
}