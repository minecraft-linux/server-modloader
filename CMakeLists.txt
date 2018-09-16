cmake_minimum_required(VERSION 3.6)
project(server_modloader)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

set(MODLOADER_VERSION "Preview 1")

add_subdirectory(dep/funchook)

include_directories(include/)
add_library(server_modloader SHARED include/modloader/hook.h include/modloader/statichook.h include/modloader/log.h
        include/modloader/loader.h
        src/hook.cpp src/log.cpp src/loader.h src/loader.cpp src/elf_helper.cpp src/elf_helper.h src/main.cpp)
target_link_libraries(server_modloader funchook)
target_compile_definitions(server_modloader PRIVATE MODLOADER_VERSION="${MODLOADER_VERSION}")