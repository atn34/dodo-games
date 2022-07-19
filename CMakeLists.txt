cmake_minimum_required(VERSION 3.18)
set(LLVM_MOS_PLATFORM dodo)
find_package(llvm-mos-sdk REQUIRED)
project(dodo-games)

set(CMAKE_CXX_STANDARD 17)

# This is to make clangd find dodo platform include directories
unset(CMAKE_C_IMPLICIT_INCLUDE_DIRECTORIES)
unset(CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES)
include_directories(SYSTEM ${llvm-mos-sdk_DIR}/../../../mos-platform/dodo/include)
include_directories(SYSTEM ${llvm-mos-sdk_DIR}/../../../mos-platform/common/include)

add_compile_options(-mcpu=mos65c02)

add_subdirectory(menu)
add_subdirectory(fixed_point)

function(add_dodo_game game)
    get_filename_component(game_name ${game} NAME_WLE)
    add_executable(${game_name} ${game})
    set_target_properties(${game_name} PROPERTIES OUTPUT_NAME "fram.bin")
    set_target_properties(${game_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${game_name}")
endfunction()

add_dodo_game(bounce.cpp)
target_link_libraries(bounce PRIVATE menu fixed_point)
