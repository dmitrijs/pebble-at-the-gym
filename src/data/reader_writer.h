#pragma once

#include <pebble.h>

long read_long(uint8_t* buf);
void write_long(uint8_t* buf, long value);
