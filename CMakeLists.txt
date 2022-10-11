cmake_minimum_required(VERSION 3.16)
project(json_cpp)

set(CMAKE_CXX_STANDARD 20)

include_directories("/usr/local/lib")
include_directories(include)

# Header files
file (GLOB_RECURSE HEADER_FILES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/include/*.h
    ${PROJECT_SOURCE_DIR}/include/*.hpp
)

# Source files
file (GLOB_RECURSE SRC_FILES CONFIGURE_DEPENDS
    ${PROJECT_SOURCE_DIR}/src/*.c
    ${PROJECT_SOURCE_DIR}/src/*.cpp
)

add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SRC_FILES})

target_link_libraries(${PROJECT_NAME})
