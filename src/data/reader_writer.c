#include "reader_writer.h"

long read_long(uint8_t* buf) {
    long value = *(buf);
    for (int i = 1; i <= 3; i++) {
        value <<= 8;
        value += *(buf + i);
    }
    return value;
}

void write_long(uint8_t* buf, long value) {
    for (int i = 0; i <= 3; i++) {
        *(buf + i) = (uint8_t) (value >> (24 - (i * 8)) % 256);
    }
}
