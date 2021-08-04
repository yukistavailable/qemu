#include "util.h"
void find() {
    unsigned int field;
    unsigned int bar;
    unsigned short config_addr = 0xcf8;
    unsigned short config_data = 0xcfc;
    unsigned int virtio = 0x10001af4;

    int baseField = 0b10000000000000000000000000000000;
    for (int i=0;i<256;i++) {
        for (int j=0;j<32;j++) {
            field = (unsigned int) (baseField + (i << 16) + (j << 11));
            asm volatile("out %1, %0;"
            ::"Nd"(config_addr), "a"(field));
            asm volatile("in %1, %0;"
            :"=r"(bar)
            :"Nd"(config_data));
            if (bar == virtio) {
                field = field + 0x10;
                asm volatile("out %1, %0;"
                ::"Nd"(config_addr), "a"(field));
                asm volatile("in %1, %0;"
                :"=r"(bar)
                :"Nd"(config_data));
                // puth(bar, 8);
                bar = bar & 0b11111111111111111111111111111100;
                // puts("\n");
                // puth(bar, 8);
                // puts("\n");
                unsigned short bar_ = (unsigned short) bar + 0x14;

                for (int i=0;i<6;i++) {
                    unsigned char data;
                    asm volatile("inb %1, %0;"
                    :"=a"(data)
                    :"Nd"(bar_));
                    puth(data, 2);
                    bar_ = (unsigned short) bar_ + 1;
                }
                break;
            }
        }
    }
    // asm volatile("out %1, %0;"
    // ::"Nd"(config_addr), "a"(field));
    // asm volatile("in %1, %0;"
    // :"=r"(result)
    // :"Nd"(config_data));
    // puth(result, 8);
}
