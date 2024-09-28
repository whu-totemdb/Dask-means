cmake_minimum_required(VERSION 3.10)

project(DaskMeansCpp VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

include_directories(
    ${PROJECT_SOURCE_DIR}/algorithms
    ${PROJECT_SOURCE_DIR}/structure
    ${PROJECT_SOURCE_DIR}/utils
)

set(SOURCES
    utils/utils.cpp
    algorithms/KMeansBase.cpp
    algorithms/Lloyd.cpp
    interface.cpp
)

add_executable(DaskMeans ${SOURCES})
