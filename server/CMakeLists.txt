# 设置源文件列表
set(SERVER_SOURCES
    main.cpp
)

# 添加可执行文件的构建
add_executable(server ${SERVER_SOURCES})

# 链接 lib 库
target_link_libraries(server PUBLIC lib)
