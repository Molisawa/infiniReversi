cmake_minimum_required(VERSION 3.17)
#file(COPY ${CMAKE_BINARY_DIR}/gsl/src/gsl_local-build/bin DESTINATION ${CMAKE_BINARY_DIR})
add_library(ai SHARED trainer_ai.c trainer_ai.h)
add_compile_definitions(BUILD_SHARED_LIBS)
add_compile_definitions(CMAKE_RUNTIME_OUTPUT_DIRECTORY)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(BUILD_SHARED_LIBS OFF)
configure_file(CMakeLists.txt.dependency.in ${CMAKE_BINARY_DIR}/dependency/CMakeLists.txt)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/dependency" )
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/dependency" )

add_subdirectory(${CMAKE_BINARY_DIR}/gsl/src/gsl_local ${CMAKE_BINARY_DIR}/gsl/src/gsl_local-build)
target_link_libraries(ai gsl gslcblas m raylib reversi_game_engine)