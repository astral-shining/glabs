cmake_minimum_required(VERSION 3.14)

project(
    "glabs"
    DESCRIPTION ""
    HOMEPAGE_URL ""
)

add_library(${PROJECT_NAME} INTERFACE)

message(${CMAKE_PROJECT_NAME})
target_include_directories(
    ${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
)

