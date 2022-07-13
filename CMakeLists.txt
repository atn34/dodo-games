cmake_minimum_required(VERSION 3.18)
set(LLVM_MOS_PLATFORM dodo)
find_package(llvm-mos-sdk REQUIRED)
project(dodo-games)

set(CMAKE_CXX_STANDARD 17)
include_directories(SYSTEM api)
add_compile_options(-Oz)

add_subdirectory(menu)
add_subdirectory(fixed_point)

add_executable(bounce bounce.cpp)
target_link_libraries(bounce PRIVATE menu fixed_point)
set_target_properties(bounce PROPERTIES OUTPUT_NAME "fram.bin")
set_target_properties(bounce PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bounce")
