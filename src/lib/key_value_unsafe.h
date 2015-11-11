#pragma once

typedef void (*KeyValueReadCallback)(void *ctx, char *key, char *value);

void read_key_values_unsafe(void *ctx, char *data, KeyValueReadCallback cb);
