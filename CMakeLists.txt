cmake_minimum_required(VERSION 3.10)

project(WebServer)

# 添加子目录
add_subdirectory(lib)
add_subdirectory(server)
add_subdirectory(client)

add_executable(test test.cpp)
target_link_libraries(test PUBLIC lib)
