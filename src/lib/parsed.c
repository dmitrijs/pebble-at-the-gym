#include "parsed.h"

#include <pebble.h>

#define PARSED_DEBUG false

parsed *parsed_create(char *data, char delimeter) {
    parsed *p = malloc(sizeof(parsed));
    p->data = data;
    p->delimeter = delimeter;
    p->_pos = 0;
    return p;
}

size_t parsed_number(parsed *p) {
    if (PARSED_DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "parsing number here: %s", p->data + p->_pos);

    char tmp[strlen(p->data)];
    size_t tmp_i = 0;
    while (p->data[p->_pos] != p->delimeter) {
        tmp[tmp_i] = p->data[p->_pos];
        tmp_i++;
        p->_pos++;
    }
    p->_pos++;
    tmp[tmp_i] = '\0';

    if (PARSED_DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "parsed_number = %s", tmp);
    return (size_t) atoi(tmp);
}

void parsed_skip(parsed *p, int count) {
    p->_pos += count;
}

void parsed_string(parsed *p, char *dest, size_t str_len) {
    memcpy(dest, p->data + p->_pos, str_len);
    p->_pos += str_len + 1;
}

bool parsed_done(parsed *p) {
    if (PARSED_DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "strlen(p->data): %d, p->_pos: %d", strlen(p->data), p->_pos);
    return (strlen(p->data) == p->_pos) || (strlen(p->data) == p->_pos + 1 /* TODO: weird symbol at the end of p->data */);
}

void parsed_print_left(parsed *p) {
    if (PARSED_DEBUG) APP_LOG(APP_LOG_LEVEL_DEBUG, "parsed left: %s", p->data + p->_pos);
}
