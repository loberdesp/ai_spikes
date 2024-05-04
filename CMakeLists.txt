cmake_minimum_required(VERSION 3.26)
project(ai_touches_spikes)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake_modules)

set(SDL2_PATH "C:/SDL2-2.30.2/x86_64-w64-mingw32")
set(SDL2TTF_PATH "C:/SDL2-2.30.2/x86_64-w64-mingw32")
set(SDL2_IMAGE_PATH "C:/SDL2-2.30.2/x86_64-w64-mingw32")

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2TTF REQUIRED)

if(SDL2_FOUND)
    message(STATUS "SDL2 found!")
else()
    message(FATAL_ERROR "SDL2 not found!")
endif()

include_directories(${SDL2_INCLUDE_DIR} ${SDL2_IMAGE_INCLUDE_DIR} ${SDL2TTF_INCLUDE_DIR})
add_executable(ai_touches_spikes main.cpp
        cpp/Game.cpp
        headers/Game.h
        cpp/display.cpp
        headers/display.h
        headers/define.h
)

#THOSE 4 LINES TOOK ME 4 HOURS TO FIND BECAUSE PROGRAM WOULDN'T WORK WITHOUT THEM

target_link_libraries(ai_touches_spikes ${SDL2_LIBRARY} ${SDL2_IMAGE_LIBRARY} ${SDL2TTF_LIBRARY} -lwinmm -limm32 -lsetupapi -lversion)