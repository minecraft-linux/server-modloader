cmake_minimum_required(VERSION 2.8)
project(server_modloader)

include(CMakePackageConfigHelpers)

if (CMAKE_VERSION VERSION_LESS 3.0.0)
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
else()
    set(CMAKE_CXX_STANDARD 11)
endif()

set(CMAKE_POSITION_INDEPENDENT_CODE ON)

#BDS is linked to libc++ since 1.20.40
if(NOT "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  message(NOTICE "Setting compiler to clang++")
  set(CMAKE_CXX_COMPILER "clang++")
endif()
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -stdlib=libc++")

set(MODLOADER_VERSION "Preview 3")

add_subdirectory(dep/funchook)

add_library(server_modloader SHARED include/modloader/hook.h include/modloader/statichook.h include/modloader/log.h
        include/modloader/loader.h
        src/hook.cpp src/log.cpp src/loader.h src/loader.cpp src/elf_helper.cpp src/elf_helper.h src/main.cpp)
target_link_libraries(server_modloader funchook)
target_compile_definitions(server_modloader PRIVATE MODLOADER_VERSION="${MODLOADER_VERSION}")
target_include_directories(server_modloader PUBLIC include/)

install(TARGETS server_modloader DESTINATION lib)
install(DIRECTORY ${CMAKE_SOURCE_DIR}/include/ DESTINATION include)
install(FILES ModLoaderConfig.cmake DESTINATION share/cmake/modloader)
