cmake_minimum_required(VERSION 3.17)
project(infinireversi)

set(CMAKE_C_STANDARD 99)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
# Set this to the minimal version you want to support
find_package(raylib 3.0 QUIET) # Let CMake search for a raylib-config.cmake
include_directories(BEFORE libs)
add_executable(infinireversi main.c)

add_compile_definitions(SUPPORT_FILEFORMAT_GIF)
add_compile_definitions(BUILD_SHARED_LIBS)
add_compile_definitions(BUILD_EXAMPLES=0)
add_compile_definitions(SUPPORT_CAMERA_SYSTEM=0)
add_compile_definitions(SUPPORT_VR_SIMULATOR=0)
add_compile_definitions(SUPPORT_DISTORTION_SHADER=0)
add_compile_definitions(SUPPORT_IMAGE_GENERATION=0)
add_compile_definitions(SUPPORT_FILEFORMAT_DDS=0)
add_compile_definitions(SUPPORT_FILEFORMAT_HDR=0)
add_compile_definitions(SUPPORT_FILEFORMAT_KTX=0)
add_compile_definitions(SUPPORT_FILEFORMAT_ASTC=0)
add_compile_definitions(SUPPORT_FILEFORMAT_OBJ=0)
add_compile_definitions(SUPPORT_FILEFORMAT_MTL=0)
add_compile_definitions(SUPPORT_MESH_GENERATION=0)
add_compile_definitions(SUPPORT_SCREEN_CAPTURE=0)
add_compile_definitions(SUPPORT_GIF_RECORDING=0)
add_compile_definitions(CMAKE_RUNTIME_OUTPUT_DIRECTORY=${CMAKE_BINARY_DIR}/bin)


# You could change the QUIET above to REQUIRED and remove this if() clause
# This part downloads raylib and builds it if it's not installed on your system
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
    set(BUILD_EXAMPLES OFF)
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
    set(SUPPORT_FILEFORMAT_GIF ON)
    set(SUPPORT_SCREEN_CAPTURE OFF)
    set(SUPPORT_GIF_RECORDING OFF)

    # build raylib
    add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})

endif ()


file(COPY resources DESTINATION ${CMAKE_BINARY_DIR}/bin)
if (WIN32)
    set(CMAKE_SHARED_LINKER_FLAGS "-Wl,--export-all-symbols")
endif (WIN32)
add_subdirectory(libs/graphic)
add_subdirectory(libs/engine)
add_subdirectory(libs/cjson)

target_link_libraries(infinireversi graphic_engine)
