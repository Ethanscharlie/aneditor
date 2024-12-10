cmake_minimum_required(VERSION 3.20)                                                                 
project(index)       

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)

file(GLOB_RECURSE SOURCES 
  ${PROJECT_SOURCE_DIR}/src/*.cpp
  ${PROJECT_SOURCE_DIR}/imgui/*.cpp 
)

add_executable(index
  ${SOURCES}
)

target_include_directories(index PUBLIC
  ${PROJECT_SOURCE_DIR}/include/imgui
)

target_link_libraries(index SDL2::SDL2 SDL2_image::SDL2_image)

file(COPY ${CMAKE_SOURCE_DIR}/res DESTINATION ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
