cmake_minimum_required(VERSION 3.7)
project(wireless-info-icon VERSION 1.0.0 LANGUAGES CXX)

find_package(Qt5 COMPONENTS Widgets REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_subdirectory(src)

install(TARGETS wireless-info-icon DESTINATION bin)
install(CODE "execute_process(COMMAND setcap cap_net_admin=ep ${CMAKE_INSTALL_PREFIX}/bin/wireless-info-icon)")
