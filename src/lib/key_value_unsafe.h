#pragma once

typedef void (*KeyValueReadCallback)(void *ctx, char *key, char *value);

/**
* first call can be ("", 0, <key>, <value>)
*/
char *add_key_value_unsafe(char *s, size_t s_size, char *key, char *value);

char *add_key_value_int_unsafe(char *s, size_t s_size, char *key, int value);

void read_key_values_unsafe(void *ctx, char *data, KeyValueReadCallback cb);
