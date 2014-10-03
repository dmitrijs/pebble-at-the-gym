#pragma once

void persist_read_long_string(uint32_t index, char *dest);

int persist_read_long_string_length(uint32_t index);

bool persist_write_long_string(uint32_t index, char *data);

void persist_delete_long_string(uint32_t index);
