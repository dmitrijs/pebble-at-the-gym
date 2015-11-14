#include "serializer.h"

Serializer* serializer_create(uint8_t* buf) {
    Serializer* s = malloc(sizeof(Serializer));
    s->buf = buf;
    s->size = 0;
    return s;
}

void serializer_destroy(Serializer* ser) {
    ser->buf = NULL;
    free(ser);
}

long serializer_read_long(Serializer* ser) {
    uint8_t* buf = &ser->buf[ser->size];

    long value = *(buf);
    for (int i = 1; i <= 3; i++) {
        value <<= 8;
        value += *(buf + i);
    }
    ser->size += 4;
    return value;
}

void serializer_write_long(Serializer* ser, long value) {
    uint8_t* buf = &ser->buf[ser->size];

    for (int i = 0; i <= 3; i++) {
        *(buf + i) = (uint8_t) (value >> (24 - (i * 8)) % 256);
    }
    ser->size += 4;
}

uint8_t serializer_read_uint8(Serializer* ser) {
    uint8_t value = ser->buf[ser->size];
    ser->size++;
    return value;
}

void serializer_write_uint8(Serializer* ser, uint8_t value) {
    ser->buf[ser->size] = value;
    ser->size++;
}
