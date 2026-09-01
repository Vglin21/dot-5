cmake_minimum_required(VERSION 3.16)
project(dot-5 C)

set(CMAKE_C_STANDARD 11)
if (EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/web)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

file(GLOB DOT5_SOURCES src/*.c)

add_executable(dot-5 ${DOT5_SOURCES})
target_include_directories(dot-5 PRIVATE include)
target_compile_options(dot-5 PRIVATE -O2 -flto)
target_link_options(dot-5 PRIVATE -flto)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(dot-5 PRIVATE
        -sUSE_SDL=3
        -sALLOW_MEMORY_GROWTH=1
        -sFORCE_FILESYSTEM=1
        "SHELL:--pre-js ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/preload.js"
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/src/shell.html"
        -sEXPORTED_FUNCTIONS=['_main','_display_resize_web']
    )

    add_custom_command(TARGET dot-5 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rename
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dot-5.html
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
    )
else()
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/SDL3")
    find_package(SDL3 REQUIRED CONFIG)
    target_link_libraries(dot-5 PRIVATE SDL3::SDL3)

    enable_language(RC)
    target_sources(dot-5 PRIVATE src/dot-5.rc)
    target_link_options(dot-5 PRIVATE -mwindows -municode)
endif()

if (NOT EMSCRIPTEN)
    add_executable(d5asm src/d5asm/d5asm.c)
    target_include_directories(d5asm PRIVATE include)
    target_compile_options(d5asm PRIVATE -O2 -flto)
    target_link_options(d5asm PRIVATE -flto)

    if (WIN32)
        target_link_options(d5asm PRIVATE -municode)
    endif()
endif()