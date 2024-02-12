cmake_minimum_required(VERSION 3.20)

include(FetchContent)

project(simulation)

find_package(fmi4cpp CONFIG REQUIRED)

set(RAYLIB_VERSION 4.5.0)
FetchContent_Declare(
    raylib
    URL https://github.com/raysan5/raylib/archive/refs/tags/${RAYLIB_VERSION}.tar.gz
    FIND_PACKAGE_ARGS ${RAYLIB_VERSION} EXACT
)

FetchCOntent_MakeAvailable(raylib)

add_executable(
  ${PROJECT_NAME}
  src/main.cpp
)

target_link_libraries(
  ${PROJECT_NAME}
    PRIVATE
      raylib
      fmi4cpp::fmi4cpp
)
