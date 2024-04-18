# 获取当前目录下的所有源文件
file(GLOB LIB_SOURCES "*.cpp")
file(GLOB LIB_HEADERS "*.h")

# 添加库的构建
add_library(lib ${LIB_SOURCES} ${LIB_HEADERS})

# 包含当前目录作为项目的头文件目录
target_include_directories(lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
