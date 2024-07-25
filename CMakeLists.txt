cmake_minimum_required(VERSION 3.17)
project(maze C)

set(CMAKE_C_STANDARD 99)

add_executable(maze main.c stack.c stack.h queue.c queue.h map.c map.h random.c random.h game.c game.h player.c player.h point.c point.h play.c play.h matrix.c matrix.h mazedef.h)