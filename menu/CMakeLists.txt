add_library(menu STATIC menu.cpp menu.hpp)
target_include_directories(menu PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
