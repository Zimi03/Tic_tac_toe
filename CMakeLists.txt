cmake_minimum_required(VERSION 3.27)
project(MinMax)

include(FetchContent)
set(BUILD_SHARED_LIBS OFF)
FETCHCONTENT_DECLARE(SFML GIT_REPOSITORY https://github.com/SFML/SFML.git GIT_TAG 2.6.1)
FETCHCONTENT_MAKEAVAILABLE(SFML)

set(CMAKE_CXX_STANDARD 17)

add_executable(MinMax main.cpp Game.hpp)

target_link_libraries(MinMax PRIVATE sfml-graphics)
