cmake_minimum_required(VERSION 3.6)
project(server_modloader)

set(CMAKE_CXX_STANDARD 11)

add_subdirectory(dep/funchook)

include_directories(include/)
add_library(server_modloader include/modloader/hook.h include/modloader/statichook.h include/modloader/log.h
        src/hook.cpp src/log.cpp)
target_link_libraries(server_modloader funchook)