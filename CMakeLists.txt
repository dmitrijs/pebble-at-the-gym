cmake_minimum_required(VERSION 2.8.4)
project(at_the_gym)

set(SOURCE_FILES appinfo.json
    resources/icon.png
    src/at_the_gym.c
)
add_executable(at_the_gym ${SOURCE_FILES})
