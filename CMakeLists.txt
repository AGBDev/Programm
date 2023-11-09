cmake_minimum_required(VERSION 3.25)

project(Programm)

# compiled with g, for debugging. May remove later
add_compile_options(-static-libgcc -W -g -O)
add_executable(prog src/main.c src/util.c src/files/files.c)
