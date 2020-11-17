cmake_minimum_required(VERSION 3.17)
project(infinireversi)

set(CMAKE_C_STANDARD 99)

add_library(reversi SHARED reversi.c reversi.h)

# Set this to the minimal version you want to support
find_package(raylib 3.0 QUIET) # Let CMake search for a raylib-config.cmake

# You could change the QUIET above to REQUIRED and remove this if() clause
# This part downloads raylib and builds it if it's not installed on your system
if (NOT raylib_FOUND) # If there's none, fetch and build raylib
    include(FetchContent)

    FetchContent_Declare(
            raylib
            URL https://github.com/raysan5/raylib/archive/master.tar.gz
    )

    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)

        set(BUILD_SHARED_LIBS ON)
        set(SUPPORT_HIGH_DPI ON)
        set(SUPPORT_CAMERA_SYSTEM OFF)
        set(SUPPORT_CAMERA_SYSTEM OFF)
        set(SUPPORT_VR_SIMULATOR OFF)
        set(SUPPORT_DISTORTION_SHADER OFF)
        set(SUPPORT_IMAGE_GENERATION OFF)
        set(SUPPORT_FILEFORMAT_DDS OFF)
        set(SUPPORT_FILEFORMAT_HDR OFF)
        set(SUPPORT_FILEFORMAT_KTX OFF)
        set(SUPPORT_FILEFORMAT_ASTC OFF)
        set(SUPPORT_FILEFORMAT_OBJ OFF)
        set(SUPPORT_FILEFORMAT_MTL OFF)
        set(SUPPORT_MESH_GENERATION OFF)
        set(USE_AUDIO OFF)
        set(SUPPORT_SCREEN_CAPTURE OFF)
        set(SUPPORT_GIF_RECORDING OFF)

        # build raylib
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

    endif()

endif()

add_executable(infinireversi main.c)
target_link_libraries(infinireversi reversi)
add_custom_command(TARGET infinireversi POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${raylib_BINARY_DIR}/src" ${CMAKE_BINARY_DIR})
target_link_libraries(infinireversi raylib)