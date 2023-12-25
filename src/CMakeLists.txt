cmake_minimum_required(VERSION 3.7)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -pedantic -Wno-unused-parameter")

add_executable(wireless-info-icon
    main.cpp
    Icon.cpp
    Icon.hpp
    Controller.cpp
    Controller.hpp
    Config.hpp
)

target_link_libraries(wireless-info-icon Qt5::Widgets)
