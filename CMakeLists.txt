cmake_minimum_required(VERSION 3.0.0)
project(Reflect VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include/)

file(GLOB_RECURSE SOURCE_CPP ./src/*.cpp)
file(GLOB_RECURSE SOURCE_C ./src/*.c)

add_library(Reflect SHARED ${SOURCE_C} ${SOURCE_CPP})

file(GLOB_RECURSE TEST_CPP ./test/*.cpp)
file(GLOB_RECURSE TEST_C ./test/*.c)

add_executable(testReflect ${TEST_C} ${TEST_CPP})
target_link_directories(testReflect PUBLIC ./build/)
target_link_libraries(testReflect Reflect)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
