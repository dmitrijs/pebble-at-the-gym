#pragma once

#include <pebble.h>

typedef struct Serializer Serializer;
struct Serializer {
    uint8_t* buf;
    uint8_t size;
};

Serializer* serializer_create(uint8_t* buf);
void serializer_destroy(Serializer* ser);

long serializer_read_long(Serializer* serializer);
void serializer_write_long(Serializer* serializer, long value);

uint8_t serializer_read_uint8(Serializer* serializer);
void serializer_write_uint8(Serializer* serializer, uint8_t value);
