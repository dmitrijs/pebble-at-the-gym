#include "tests.h"
#include "../data/machine.h"

static bool types_are_right_length() {
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "long size: %d", sizeof(long));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "int size: %d", sizeof(int));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "double size: %d", sizeof(double));
//    APP_LOG(APP_LOG_LEVEL_DEBUG, "float size: %d", sizeof(float));

    return (sizeof(long) == 4) && (sizeof(int) == 4);
}

static bool workout_serializes() {
    Workout *w = workout_create();

    w->location = 'X';
    w->time_start = 1234567;
    w->time_end = 1234567890;

    uint8_t n = 0;
    Machine *m = w->first_machine;
    while (m != NULL) {
        m->warmup_kg = (uint8_t) (n * 2 + 1);
        m->normal_kg = (uint8_t) (n * 3 + 1);

        m->set_1 = (uint8_t) (n * 4 + 1);
        m->set_2 = (uint8_t) (n * 4 + 2);
        m->set_3 = (uint8_t) (n * 4 + 3);

        m->is_done = (uint8_t) ((n % 3 == 1) ? 1 : 0);
        m->time_done = 1234000 + n;

        m = m->next;
        n++;
    }

    uint8_t buf[256];

    workout_serialize_for_test(buf, w);

    workout_destroy(w);

    uint8_t dst[256];
    memcpy(dst, buf, 256);

    Workout *w2 = workout_create();

    workout_unserialize_for_test(dst, w2);

    bool success = true;

    if (w->location != w2->location ||
        w->time_start != w2->time_start ||
        w->time_end != w2->time_end) {

        success = false;
    }

    n = 0;
    Machine *m2 = w->first_machine;
    while (m2 != NULL) {
        if (m2->warmup_kg != (uint8_t) (n * 2 + 1) ||
            m2->normal_kg != (uint8_t) (n * 3 + 1) ||
            m2->set_1 != (uint8_t) (n * 4 + 1) ||
            m2->set_2 != (uint8_t) (n * 4 + 2) ||
            m2->set_3 != (uint8_t) (n * 4 + 3) ||
            m2->is_done != (uint8_t) ((n % 3 == 1) ? 1 : 0) ||
            m2->time_done != 1234000 + n) {

            success = false;
        }

        m2 = m2->next;
        n++;
    }

    workout_destroy(w2);
    return success;
}

bool tests_pass() {
    return types_are_right_length() && workout_serializes();
}
