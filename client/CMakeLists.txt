# 设置源文件列表
set(CLIENT_SOURCES
    main.cpp
)

# 添加可执行文件的构建
add_executable(client ${CLIENT_SOURCES})

# 链接 lib 库
target_link_libraries(client PUBLIC lib)
