cmake_minimum_required(VERSION 2.8.4)
project(at_the_gym)

set(SOURCE_FILES appinfo.json
    resources/icon.png
    resources/prepare_bg.png

    src/js/pebble-js-app.js

    src/main.c

    src/data/machine.c src/data/machine.h

    src/lib/parsed.c src/lib/parsed.h
    src/lib/key_value_unsafe.c src/lib/key_value_unsafe.h
    src/lib/message-queue.c src/lib/message-queue.h

    src/ui/gym/window_with_timer.c src/ui/gym/window_with_timer.h
    src/ui/gym/window_location.c src/ui/gym/window_location.h
    src/ui/gym/window_menu.c src/ui/gym/window_menu.h
    src/ui/prepare/window_prepare.c src/ui/prepare/window_prepare.h
    src/ui/upload/window_upload.c src/ui/upload/window_upload.h
    src/ui/window_error.c src/ui/window_error.h
)
add_executable(at_the_gym ${SOURCE_FILES})
