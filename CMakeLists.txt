cmake_minimum_required(VERSION 3.2)

project(CapStoneProject VERSION 1.0 HOMEPAGE_URL "https://github.com/JR-Rong/BH-CapstoneProject")

if(CMAKE_HOST_UNIX)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
elseif(CMAKE_HOST_WIN32)
    set(CMAKE_CXX_FLAGS)    
endif()

set(CMAKE_CXX_STANDARD 17)

file(
    GLOB_RECURSE SOURCE_CPP 
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)
message("Find Source File: ${SOURCE_CPP}")
add_executable(
    app
    ${SOURCE_CPP}
)

include_directories(${PROJECT_SOURCE_DIR}/include)
