
# CMake 最低版本号要求
cmake_minimum_required (VERSION 2.8)

# 项目信息
project (Demo1)

# 指定生成目标
# 查找目录下的所有源文件
aux_source_directory(. DIR_SRCS)

INCLUDE_DIRECTORIES(${PROJECT_SOURCE_DIR}/include)

# 添加math 子目录，使得math子目录的cmakelists可以被执行
add_subdirectory(math)

# 指定生成目标
add_executable(Demo ${DIR_SRCS})

# 添加链接库
target_link_libraries(Demo MathFunctions)
