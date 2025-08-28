cmake_minimum_required(VERSION 3.10..4.0)
project(Dimentia LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)
FetchContent_Declare(
    raygui
    GIT_REPOSITORY https://github.com/raysan5/raygui.git
    GIT_TAG master
)
FetchContent_MakeAvailable(raylib raygui)

include_directories(${raygui_SOURCE_DIR}/src)
file(GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS "${CMAKE_SOURCE_DIR}/src/*.cpp")

add_executable(Dimentia ${SRC_FILES})

target_include_directories(Dimentia PRIVATE ${CMAKE_SOURCE_DIR}/include)

target_link_libraries(Dimentia PRIVATE raylib)
