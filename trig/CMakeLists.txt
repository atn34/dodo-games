add_library(trig STATIC trig.cpp trig.hpp)
target_include_directories(trig PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(trig PRIVATE fixed_point)
