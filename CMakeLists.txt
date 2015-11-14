cmake_minimum_required(VERSION 2.8.4)
project(at_the_gym)

include_directories(build/basalt/src)

set(HEADER_FILES build/basalt/src/resource_ids.auto.h)

set(SOURCE_FILES appinfo.json
        resources/icon.png
        resources/prepare_bg.png

        src/js/pebble-js-app.js

        src/main.c

        src/data/machine.c src/data/machine.h
        src/tests/tests.c src/tests/tests.h

        src/lib/key_value_unsafe.c src/lib/key_value_unsafe.h
        src/lib/message-queue.c src/lib/message-queue.h

        src/ui_gym/window_with_timer.c src/ui_gym/window_with_timer.h
        src/ui_gym/window_location.c src/ui_gym/window_location.h
        src/ui_gym/window_menu.c src/ui_gym/window_menu.h
        src/ui_prepare/window_prepare.c src/ui_prepare/window_prepare.h
        src/ui_upload/window_upload.c src/ui_upload/window_upload.h
        src/ui_delete/window_delete.c src/ui_delete/window_delete.h
        src/ui/window_error.c src/ui/window_error.h
        )
add_executable(at_the_gym ${SOURCE_FILES})
