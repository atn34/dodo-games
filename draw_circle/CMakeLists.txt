add_library(draw_circle STATIC draw_circle.cpp draw_circle.hpp)
target_include_directories(draw_circle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
