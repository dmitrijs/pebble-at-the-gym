#include "tests.h"

static bool long_is_right_length() {
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "long size: %d", sizeof(long));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "int size: %d", sizeof(int));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "double size: %d", sizeof(double));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "float size: %d", sizeof(float));

    return sizeof(long) == 4;
}

static bool serialization_works() {
    uint8_t buf[5];

    long x = 1234567890;
    buf[0] = (uint8_t)(x >> 24 % 256);
    buf[1] = (uint8_t)(x >> 16 % 256);
    buf[2] = (uint8_t)(x >> 8 % 256);
    buf[3] = (uint8_t)(x >> 0 % 256);

    uint8_t dst[5];
    memcpy(dst, buf, 5);

    long x2 = 0;
    x2 += dst[0];
    x2 <<= 8;
    x2 += dst[1];
    x2 <<= 8;
    x2 += dst[2];
    x2 <<= 8;
    x2 += dst[3];

    return x == x2;
}

bool tests_pass() {
    return serialization_works() && long_is_right_length();
}
