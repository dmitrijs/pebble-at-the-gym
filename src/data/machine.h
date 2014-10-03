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
    M_ABS,
    M_ABS_SIDE,
    M_COOLDOWN,
    M__COUNT
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
    DATA_WORKOUT_CURRENT = 0, // len + chunk_size + 16 chunks = 18 indices
    DATA_WORKOUT_SAVE_1 = 20,
    DATA_WORKOUT_SAVE_2 = 30,
    DATA_WORKOUT_SAVE_3 = 40
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

    char *warmup_kg_str;
    char *normal_kg_str;
    char *set_1_str;
    char *set_2_str;
    char *set_3_str;

    bool is_done;
    long time_done;

    Machine *next;
    Machine *prev;
};

typedef struct Workout Workout;
struct Workout {
    long time_start;
    long time_end;

    int location;

    Machine *first_machine;
};

Workout *workout_create();

void workout_load_current(Workout *);

void workout_save_current(Workout *);

bool workout_try_backup(Workout *);

void workout_destroy(Workout *);
