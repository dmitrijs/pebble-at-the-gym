cmake_minimum_required(VERSION 2.8.4)
project(at_the_gym)

set(SOURCE_FILES appinfo.json
    resources/icon.png
    resources/prepare_bg.png

    src/main.c

    src/data/machine.c src/data/machine.h

    src/lib/parsed.c src/lib/parsed.h
    src/lib/key_value_unsafe.c src/lib/key_value_unsafe.h

    src/window_with_timer.c src/window_with_timer.h
    src/window_location.c src/window_location.h
    src/window_menu.c src/window_menu.h
    src/window_prepare.c src/window_prepare.h
)
add_executable(at_the_gym ${SOURCE_FILES})
