#include "tests.h"

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
    for (int i = 0; i <= 3; i++) {
        *(buf + i) = (uint8_t) (x >> (24 - (i * 8)) % 256);
    }

    uint8_t dst[4];
    memcpy(dst, buf, 4);

    long x2 = *(dst);
    for (int i = 1; i <= 3; i++) {
        x2 <<= 8;
        x2 += *(dst + i);
    }

    return x == x2;
}

bool tests_pass() {
    return serialization_works() && long_is_right_length();
}
