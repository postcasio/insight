cmake_minimum_required(VERSION 3.28)
project(Revelation)

set(CMAKE_CXX_STANDARD 20)

add_executable(Revelation src/main.cpp)

find_package(Vulkan REQUIRED)

target_include_directories(Insight PUBLIC ${Vulkan_INCLUDE_DIRS})
target_link_libraries(Insight Vulkan::Vulkan)

find_package(glfw3 3.3 REQUIRED)

target_include_directories(Insight PUBLIC ${GLFW3_INCLUDE_DIR})
target_link_libraries(Insight glfw)

message(GLFW: ${GLFW3_LIBRARY})