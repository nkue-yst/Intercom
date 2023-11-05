# CMakeLists.txt
# Author: Yoshito Nakaue
# Create: 2023/11/04
# Last Modified: 2023/11/04

cmake_minimum_required(VERSION 3.20)

project(Intercom)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CXX_FLAGS "-Wall")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake/sdl2)

### Setup main executable ###
set(MAIN_SRC_DIR ${CMAKE_SOURCE_DIR}/source)
add_executable(Intercom 
    ${MAIN_SRC_DIR}/main.cpp
    ${MAIN_SRC_DIR}/GUI.cpp
)

### SDL2 ###
find_package(SDL2 REQUIRED)
target_include_directories(Intercom PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(Intercom ${SDL2_LIBRARIES})

### SDL2_image ###
find_package(SDL2_image REQUIRED)
target_include_directories(Intercom PRIVATE ${SDL2_IMAGE_INCLUDE_DIRS})
target_link_libraries(Intercom ${SDL2_IMAGE_LIBRARIES})

### SDL2_ttf ###
find_package(SDL2_ttf REQUIRED)
target_include_directories(Intercom PRIVATE ${SDL2_TTF_INCLUDE_DIRS})
target_link_libraries(Intercom ${SDL2_TTF_LIBRARIES})
