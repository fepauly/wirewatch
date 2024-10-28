cmake_minimum_required(VERSION 3.23)

project(WireWatch C)

include_directories(include)

add_library(pretty_lib STATIC src/pretty.c)
add_library(netstuff_lib STATIC src/netstuff.c)

add_executable(wiwa src/wirewatch.c)
target_link_libraries(wiwa pretty_lib netstuff_lib)

set_target_properties(wiwa PROPERTIES
    C_STANDARD 11
)

target_compile_options(wiwa PRIVATE -Wall -Wextra -pedantic)
