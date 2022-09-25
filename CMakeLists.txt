cmake_minimum_required(VERSION 3.18)
set(LLVM_MOS_PLATFORM dodo)
find_package(llvm-mos-sdk REQUIRED)
project(dodo-games)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-mcpu=mos65c02)
# add_link_options(-Wl,--lto-emit-asm)

add_subdirectory(menu)
add_subdirectory(fixed_point)
add_subdirectory(draw_circle)
add_subdirectory(trig)
add_subdirectory(string)

function(add_dodo_game game)
    get_filename_component(game_name ${game} NAME_WLE)
    add_executable(${game_name} ${game})
    set_target_properties(${game_name} PROPERTIES OUTPUT_NAME "fram.bin")
    set_target_properties(${game_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${game_name}")
endfunction()

add_dodo_game(bounce.cpp)
target_link_libraries(bounce PRIVATE menu fixed_point string)

add_dodo_game(clock.cpp)
target_link_libraries(clock PRIVATE draw_circle trig)

add_subdirectory(mercenary)
