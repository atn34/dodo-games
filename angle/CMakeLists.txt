add_library(angle STATIC angle.cpp angle.hpp)
target_include_directories(angle PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(angle PRIVATE fixed_point)
