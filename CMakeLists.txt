cmake_minimum_required(VERSION 3.1)

project(lab8)

find_package(Threads)

add_executable(lab8
    main.cpp
    point.cpp
    rhombus.cpp
    pentagon.cpp
    hexagon.cpp)

set_property(TARGET lab8 PROPERTY CXX_STANDARD 17)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall -Wextra -Werror")

target_link_libraries(lab8 ${CMAKE_THREAD_LIBS_INIT})
