cmake_minimum_required(VERSION 3.0.0)
project(Reflect VERSION 0.1.0)

include(CTest)
enable_testing()

include_directories(./include/)
include_directories(../MyUtils/include/)
include_directories(../Serial/include/)
include_directories(../../nlohmann/json/include/)

file(GLOB_RECURSE TEST_CPP ./test/*.cpp)
file(GLOB_RECURSE TEST_C ./test/*.c)

add_executable(testReflect ${TEST_C} ${TEST_CPP})

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
