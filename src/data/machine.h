#pragma once

#include <stdbool.h>

enum MACHINE_TYPES {
    M_WARMUP,
    M_SHOULDERS,
    M_CHEST,
    M_TRICEPS,
    M_BICEPS,
    M_UPPER_BACK,
    M_LOWER_BACK,
    M_LEGS_ULTIMATE,
//    M_LEGS_UP, /* excluded for now */
//    M_LEGS_DOWN,
            M_ABS,
    M_ABS_SIDE,
    M_COOLDOWN
};
enum FIELD_TYPE {
    F_TITLE,
    F_WARMUP_KG,
    F_NORMAL_KG,
    F_SET_1,
    F_SET_2,
    F_SET_3,
    F__COUNT
};
enum {
    DATA_MACHINES = 0
};

typedef struct Machine Machine;
struct Machine {
    int mkey;
    char *title;
    int warmup_kg;
    int normal_kg;
    int set_1;
    int set_2;
    int set_3;
    bool is_done;

    char *warmup_kg_str;
    char *normal_kg_str;
    char *set_1_str;
    char *set_2_str;
    char *set_3_str;

    Machine *next;
    Machine *prev;
};

void machines_data_load(Machine *first_machine);

void machines_data_save(Machine *first_machine);

void machines_destroy(Machine *first_machine);

Machine *machines_create_all();
