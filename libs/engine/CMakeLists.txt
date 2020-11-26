cmake_minimum_required(VERSION 3.17)
project(reversi_game_engine)

add_library(reversi_game_engine SHARED reversi_game_engine.c reversi_game_engine.h)
target_link_libraries(reversi_game_engine cjson)

