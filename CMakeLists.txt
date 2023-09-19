cmake_minimum_required(VERSION 3.25)

project(Programm)

add_compile_options(-static-libgcc -W -g)
add_executable(prog src/main.c src/util.c)