cmake_minimum_required(VERSION 3.25)

project(HalfLife3)

add_compile_options(-static-libgcc -W)
add_executable(hl3 src/main.c src/rebuilder.c src/util.c)