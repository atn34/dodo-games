add_library(fixed_point STATIC fixed_point.cpp fixed_point.hpp)
target_include_directories(fixed_point PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
