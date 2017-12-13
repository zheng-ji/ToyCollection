# 查找当前目录下的所有源文件
# 并将名称保存到 DIR_LIB_SRCS
aux_source_directory(. DIR_LIB_SRCS)

#生成链接库, 将 src 目录中的源文件编译为静态链接库
add_library(MathFunctions ${DIR_LIB_SRCS})
