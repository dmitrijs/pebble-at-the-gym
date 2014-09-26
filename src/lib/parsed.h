#pragma once

#include <stdlib.h>
#include <stdbool.h>

typedef struct {
    char *data;
    char delimeter;
    size_t _pos;
} parsed;

parsed *parsed_create(char *data, char delimeter);

size_t parsed_number(parsed *p);

void parsed_skip(parsed *p, int count);

void parsed_string(parsed *p, char *dest, size_t str_len);

bool parsed_done(parsed *p);

void parsed_print_left(parsed *p);
