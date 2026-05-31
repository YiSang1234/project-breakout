cmake_minimum_required(VERSION 3.10)
project(Breakout)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 如果使用 MSVC，添加 /utf-8 避免编码警告
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 查找 raylib 库（优先使用 CMake 配置，否则手动指定）
find_package(raylib QUIET)
if(NOT raylib_FOUND)
    message(STATUS "raylib not found via CMake, using manual paths")
    # 请根据实际安装路径修改以下两行
    set(RAYLIB_INCLUDE_DIR "C:/msys64/mingw64/include")
    set(RAYLIB_LIBRARY "C:/msys64/mingw64/lib/libraylib.a")
    include_directories(${RAYLIB_INCLUDE_DIR})
    set(RAYLIB_LIBRARIES ${RAYLIB_LIBRARY})
else()
    include_directories(${raylib_INCLUDE_DIRS})
    set(RAYLIB_LIBRARIES raylib)
endif()

# 查找 ENet 库
find_package(ENet QUIET)
if(NOT ENet_FOUND)
    message(STATUS "ENet not found via CMake, using manual paths")
    # 请根据实际安装路径修改
    set(ENET_INCLUDE_DIR "C:/msys64/mingw64/include/enet")
    set(ENET_LIBRARY "C:/msys64/mingw64/lib/libenet.a")
    include_directories(${ENET_INCLUDE_DIR})
    set(ENET_LIBRARIES ${ENET_LIBRARY})
else()
    include_directories(${ENET_INCLUDE_DIRS})
    set(ENET_LIBRARIES ${ENET_LIBRARIES})
endif()

# 添加头文件目录
include_directories(include)

# 收集源文件
file(GLOB SOURCES "src/*.cpp")

# 生成可执行文件
add_executable(Breakout ${SOURCES})

# 链接库
target_link_libraries(Breakout ${RAYLIB_LIBRARIES} ${ENET_LIBRARIES})

# Windows 需要附加的库
if(WIN32)
    target_link_libraries(Breakout ws2_32 winmm)
endif()

# 可选：复制 assets 和 levels 到构建目录
file(COPY assets/ DESTINATION ${CMAKE_BINARY_DIR}/assets)
file(COPY levels/ DESTINATION ${CMAKE_BINARY_DIR}/levels)