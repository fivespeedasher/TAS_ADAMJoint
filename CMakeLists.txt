cmake_minimum_required(VERSION 3.22)

# 项目信息
project(TAS_LAN_464E)

# 添加路径下所有源文件到DIR_SRCS变量
aux_source_directory(ADAMJointDB3.cpp DIR_SRCS)

# 添加子目录
add_subdirectory(include)

# 设定主文件
set(MAIN_FILE ADAMJointDB3.cpp)

# 指定生成目标
add_executable(run ${DIR_SRCS} ${MAIN_FILE})

# 加入多个链接库
target_link_libraries(run IncludeSrc modbus)