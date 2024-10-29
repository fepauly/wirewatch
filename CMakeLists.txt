cmake_minimum_required(VERSION 3.23)

project(WireWatch C)

include_directories(include)

add_library(pretty STATIC src/pretty.c)
add_library(commands STATIC src/commands.c)

add_executable(wiwa src/wirewatch.c)
target_link_libraries(wiwa pretty commands)

set_target_properties(wiwa PROPERTIES
    C_STANDARD 11
)

target_compile_options(wiwa PRIVATE -Wall -Wextra -pedantic)
