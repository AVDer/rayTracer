cmake_minimum_required(VERSION 3.17)
project(rayTracer VERSION 1.0
        LANGUAGES CXX)

add_executable(rayTracer main.cpp)

#target_include_directories(rayTracer PUBLIC include)
#target_link_libraries(rayTracer PUBLIC X11 Xft pthread fontconfig)

set(CMAKE_CXX_FLAGS "-Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g")
set(CMAKE_VERBOSE_MAKEFILE ON)
set_target_properties(rayTracer PROPERTIES CXX_STANDARD 20)