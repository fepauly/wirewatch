cmake_minimum_required(VERSION 3.23)

project(WireWatch C)

include_directories(include)

set(SOURCES
    src/helpers.c
    src/commands.c
    src/gateway.c
    src/pretty.c
    src/mac.c
    src/ping.c
    src/traceroute.c
    src/wirewatch.c
    src/scan.c
)

add_executable(wiwa ${SOURCES})
set_target_properties(wiwa PROPERTIES
    C_STANDARD 11
)

target_compile_options(wiwa PRIVATE -Wall -Wextra -pedantic)
