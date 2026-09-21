cmake_minimum_required(VERSION 3.16)
project(dot-8 C)

set(CMAKE_C_STANDARD 11)
if (EMSCRIPTEN)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/web)
else()
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin)
endif()

if (UNIX)
    set(CMAKE_SKIP_BUILD_RPATH FALSE)
    set(CMAKE_BUILD_WITH_INSTALL_RPATH TRUE)
    set(CMAKE_INSTALL_RPATH "$ORIGIN")
    set(CMAKE_INSTALL_RPATH_USE_LINK_PATH TRUE)
endif()

file(GLOB DOT8_SOURCES src/*.c)

add_executable(dot-8 ${DOT8_SOURCES})
target_include_directories(dot-8 PRIVATE include)
target_compile_options(dot-8 PRIVATE -O2 -flto)
target_link_options(dot-8 PRIVATE -flto)

if (EMSCRIPTEN)
    set(CMAKE_EXECUTABLE_SUFFIX ".html")
    target_link_options(dot-8 PRIVATE
        -sUSE_SDL=3
        -sALLOW_MEMORY_GROWTH=1
        -sFORCE_FILESYSTEM=1
        "SHELL:--pre-js ${CMAKE_SOURCE_DIR}/src/web/preload.js"
        "SHELL:--shell-file ${CMAKE_SOURCE_DIR}/src/web/shell.html"
        -sEXPORTED_FUNCTIONS=['_main','_display_resize_web']
    )

    add_custom_command(TARGET dot-8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E rename
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dot-8.html
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/index.html
    )
    add_custom_command(TARGET dot-8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/web/rom.d8
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rom.d8
    )
    add_custom_command(TARGET dot-8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/src/web/dot-8.cfg
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/dot-8.cfg
    )
    add_custom_command(TARGET dot-8 POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
            ${CMAKE_SOURCE_DIR}/assets/icon.ico
            ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/favicon.ico
    )
else()
    list(APPEND CMAKE_PREFIX_PATH "${CMAKE_SOURCE_DIR}/SDL3")
    find_package(SDL3 REQUIRED CONFIG)
    target_link_libraries(dot-8 PRIVATE SDL3::SDL3)

    if (WIN32)
        enable_language(RC)
        target_sources(dot-8 PRIVATE src/dot-8.rc)
        target_link_options(dot-8 PRIVATE -mwindows -municode)
    endif()
endif()