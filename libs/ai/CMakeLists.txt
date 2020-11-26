cmake_minimum_required(VERSION 3.17)
project(ai)

add_library(ai STATIC trainer_ai.c trainer_ai.h)
target_link_libraries(ai raylib)