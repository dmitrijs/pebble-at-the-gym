#include <pebble.h>
#include "key_value_unsafe.h"

#define DEBUG_KEY_VALUE false

/**
* first call can be ("", 0, <key>, <value>)
*/
char *add_key_value_unsafe(char *s, size_t s_size, char *key, char *value) {
    if (DEBUG_KEY_VALUE) APP_LOG(APP_LOG_LEVEL_DEBUG, "add key/value: %s -> %s", key, value);

    size_t additional_len = strlen(key) + 1 + strlen(value) + 1;
    size_t needed_len = strlen(s) + additional_len + 1;
    if (s_size < needed_len) {
        if (DEBUG_KEY_VALUE) APP_LOG(APP_LOG_LEVEL_DEBUG, "increasing s length from %d to %d", (int) s_size, (int) needed_len);
        char *s2 = malloc(needed_len);
        strcpy(s2, s);
        if (s_size != 0) free(s);
        s = s2;
    }
    char *key_val = malloc(additional_len + 1);
    snprintf(key_val, additional_len + 1, "%s=%s;", key, value);
    if (DEBUG_KEY_VALUE) APP_LOG(APP_LOG_LEVEL_DEBUG, "add key_val = %s", key_val);

    strcat(s, key_val);
    free(key_val);

    return s;
}

char *add_key_value_int_unsafe(char *s, size_t s_size, char *key, int value) {
    char *si = malloc(10);
    snprintf(si, 10, "%d", value);
    char *result = add_key_value_unsafe(s, s_size, key, si);
    free(si);
    return result;
}

void read_key_values_unsafe(void *ctx, char *data, KeyValueReadCallback cb) {
    if (data == NULL) {
        return;
    }

    size_t len = strlen(data);
    char *key_tmp = malloc(200);
    char *val_tmp = malloc(200);
    size_t key_i = 0, val_i = 0;
    bool in_key = true;
    for (size_t i = 0; i < len; i++) {
        if (data[i] == '=') {
            in_key = false;
            key_tmp[key_i] = '\0';
            continue;
        }
        if (data[i] == ';') {
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
    free(key_tmp);
    free(val_tmp);
}
