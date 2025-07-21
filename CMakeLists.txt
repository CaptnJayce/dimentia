cmake_minimum_required(VERSION 3.10..4.0)
project(Dimentia CXX)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)
add_executable(Dimentia
    src/Main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/AnimationManager.cpp
    src/GUI.cpp
    src/SpatialGrid.cpp
    src/Globals.cpp
)

target_link_libraries(Dimentia PRIVATE raylib)
