cmake_minimum_required(VERSION 3.25)

project(Programm)

add_compile_options(-static-libgcc -W)
add_executable(prog src/main.c src/util.c src/scp_file_format.c)