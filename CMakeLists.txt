cmake_minimum_required(VERSION 2.8.4)
project(at_the_gym)

set(SOURCE_FILES appinfo.json
    resources/icon.png
    resources/up.png resources/down.png resources/ok.png
    src/main.c
    src/at_the_gym.c
    src/window_with_timer.c src/window_with_timer.h
    src/window_edit_number.c src/window_edit_number.h
    src/lib/parsed.c src/lib/parsed.h
)
add_executable(at_the_gym ${SOURCE_FILES})
