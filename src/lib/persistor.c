#include <pebble.h>
#include "persistor.h"

#define PERSISTOR_MAX_CHUNK_LENGTH (PERSIST_STRING_MAX_LENGTH - 6)
#define PERSISTOR_MAX_CHUNKS 16

void persist_delete_long_string(uint32_t index) {
    persist_delete(index);
    persist_delete(index + 1);
    for (uint32_t i = 0; i < PERSISTOR_MAX_CHUNKS; i++) {
        persist_delete(index + 2 + i);
    }
}

bool persist_write_long_string(uint32_t index, char *data) {
    persist_delete_long_string(index);

    size_t len = strlen(data);
    if (len > PERSISTOR_MAX_CHUNKS * PERSISTOR_MAX_CHUNK_LENGTH) {
        APP_LOG(APP_LOG_LEVEL_DEBUG, "data length (%d) exceeds persistence limit", len);
        return false;
    }

    persist_write_int(index, len);
    persist_write_int(index + 1, PERSISTOR_MAX_CHUNK_LENGTH);

    char *tmp[PERSISTOR_MAX_CHUNKS];

    uint32_t num_chunks = 0;
    while (PERSISTOR_MAX_CHUNK_LENGTH * num_chunks < len) {

        tmp[num_chunks] = malloc(PERSISTOR_MAX_CHUNK_LENGTH + 1);
        strncpy(tmp[num_chunks], data + (PERSISTOR_MAX_CHUNK_LENGTH * num_chunks), PERSISTOR_MAX_CHUNK_LENGTH);
        APP_LOG(APP_LOG_LEVEL_DEBUG, "chnk=%s", tmp[num_chunks]);

        persist_write_string(index + 2 + num_chunks, tmp[num_chunks]);

        free(tmp[num_chunks]);
        num_chunks++;
    }

    APP_LOG(APP_LOG_LEVEL_DEBUG, "data written. length: %d, chunks: %d", len, (int) num_chunks);
    return true;
}

int persist_read_long_string_length(uint32_t index) {
    return persist_read_int(index);
}

void persist_read_long_string(uint32_t index, char *dest) {
    size_t len = (size_t) persist_read_int(index);

    size_t chunk_len = (size_t) persist_read_int(index + 1);

    uint32_t chunk_index = 0;
    while (chunk_index * chunk_len < len) {
        persist_read_string(index + 2 + chunk_index, dest + (chunk_index * chunk_len), chunk_len + 1);

        chunk_index++;
    }
}
