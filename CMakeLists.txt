cmake_minimum_required(VERSION 3.10..4.0)
project(Dimentia LANGUAGES C CXX)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

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

add_executable(Dimentia
    src/Main.cpp
    src/Player.cpp
    src/Enemy.cpp
    src/AnimationManager.cpp
    src/GUI.cpp
    src/SpatialGrid.cpp
    src/Globals.cpp
    src/Weapon.cpp
)

target_link_libraries(Dimentia PRIVATE raylib)