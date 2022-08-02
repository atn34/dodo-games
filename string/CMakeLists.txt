add_library(string STATIC string.cpp string.hpp)
target_include_directories(string PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
