cmake_minimum_required(VERSION 3.8)
project(glad LANGUAGES C)

# Automatically choose STATIC or SHARED based on BUILD_SHARED_LIBS
add_library(glad src/glad.c)
target_include_directories(glad PUBLIC include)

# If building as a shared library, set up Windows export macros
if(BUILD_SHARED_LIBS)
    target_compile_definitions(glad PRIVATE GLAD_GLAPI_EXPORT GLAD_GLAPI_EXPORT_BUILD)
else()
endif()
