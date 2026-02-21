cmake_minimum_required(VERSION 3.20)

project(OliUI
    VERSION 1.0
    LANGUAGES C
)

add_library(OliUI SHARED
    OliUI.c
)

# Include directories
target_include_directories(OliUI
    PUBLIC
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_compile_definitions(OliUI PRIVATE OLIUI_EXPORTS)