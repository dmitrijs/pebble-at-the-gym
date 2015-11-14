#include "tests.h"
#include "../data/reader_writer.h"

static bool long_is_right_length() {
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "long size: %d", sizeof(long));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "int size: %d", sizeof(int));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "double size: %d", sizeof(double));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "float size: %d", sizeof(float));

    return sizeof(long) == 4;
}

static bool serialization_works() {
    uint8_t buf[4];

    long x = 1234567890;
    write_long(buf, x);

    uint8_t dst[4];
    memcpy(dst, buf, 4);

    long x2 = read_long(dst);

    return x == x2;
}

bool tests_pass() {
    return serialization_works() && long_is_right_length();
}
