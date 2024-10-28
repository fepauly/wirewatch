cmake_minimum_required(VERSION 3.23)

project(WireWatch C)

set(SRC_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

file(GLOB SOURCES "${SRC_DIR}/*.c")

add_executable(wiwa ${SOURCES})

set_target_properties(wiwa PROPERTIES
    C_STANDARD 11
)

target_compile_options(wiwa PRIVATE -Wall -Wextra -pedantic)
