cmake_minimum_required(VERSION 3.1)

project(vcpkg-glfw3-broken-linux)

find_package(glfw3 REQUIRED)

add_executable(main main.cpp)

target_link_libraries(main PRIVATE glfw)
target_link_libraries(main PRIVATE ${GLFW_LIBRARIES})

