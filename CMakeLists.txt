cmake_minimum_required(VERSION 3.2)

project(CapStoneProject VERSION 1.0 HOMEPAGE_URL "https://github.com/JR-Rong/BH-CapstoneProject")

set(CMAKE_MAKE_PROGRAM "MinGW Makefiles")

if(CMAKE_HOST_UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
    link_directories("${PROJECT_SOURCE_DIR}/lib/lunux")
    set(OPENGL_LIB "glfw3.so")
elseif(CMAKE_HOST_WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fpermissive")   
    link_directories("${PROJECT_SOURCE_DIR}/lib/win-vs2022")
    set(OPENGL_LIB "glfw3.lib")
endif()

set(CMAKE_CXX_STANDARD 17)

add_definitions(-DPROJECT_CURRENT_DIR="${PROJECT_SOURCE_DIR}/")

file(
    GLOB_RECURSE SOURCE_CPP 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

set(
    THIRD_PARTY_CPP 
    ${PROJECT_SOURCE_DIR}/third_party/glad/glad.c
    ${PROJECT_SOURCE_DIR}/third_party/json/jsoncpp.cpp
)

message("Find Source File: ${SOURCE_CPP}")
add_executable(
    app
    ${SOURCE_CPP}
    ${THIRD_PARTY_CPP}
)

include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/third_party)

target_link_libraries(app opengl32.lib ${OPENGL_LIB})
