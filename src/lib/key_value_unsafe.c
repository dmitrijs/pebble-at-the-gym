#include <pebble.h>
#include "key_value_unsafe.h"

void read_key_values_unsafe(void *ctx, char *data, KeyValueReadCallback cb) {
    if (data == NULL) {
        return;
    }

    size_t len = strlen(data);
    char key_tmp[10];
    char val_tmp[200];
    size_t key_i = 0, val_i = 0;
    bool in_key = true;
    for (size_t i = 0; i < len; i++) {
        if (data[i] == '=') {
            in_key = false;
            key_tmp[key_i] = '\0';
            continue;
        }
        if (data[i] == ';' || data[i] == ',' || data[i] == ' ') {
            in_key = true;
            val_tmp[val_i] = '\0';

            cb(ctx, key_tmp, val_tmp);

            key_i = 0;
            val_i = 0;
            continue;
        }

        if (in_key) {
            key_tmp[key_i] = data[i];
            key_i++;
        } else {
            val_tmp[val_i] = data[i];
            val_i++;
        }
    }
}
