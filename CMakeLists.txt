cmake_minimum_required(VERSION 3.23)

project(WireWatch C)

include_directories(include)

add_executable(wiwa src/wirewatch.c src/commands.c src/pretty.c)

set_target_properties(wiwa PROPERTIES
    C_STANDARD 11
)

target_compile_options(wiwa PRIVATE -Wall -Wextra -pedantic)
