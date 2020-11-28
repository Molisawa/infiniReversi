cmake_minimum_required(VERSION 3.17)

add_library(graphic_engine SHARED graphic_engine.c graphic_engine.h)
target_link_libraries(graphic_engine raylib reversi_game_engine)