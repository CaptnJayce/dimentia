cmake_minimum_required(VERSION 3.10..4.0)
project(KalpaChaos CXX)

include(FetchContent)
FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
)

FetchContent_MakeAvailable(raylib)
add_executable(KalpaChaos 
    src/Main.cpp
    src/Player.cpp
)

target_link_libraries(KalpaChaos PRIVATE raylib)
