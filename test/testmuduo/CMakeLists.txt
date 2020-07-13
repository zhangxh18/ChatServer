#include_directories()

#link_directories()

set(SRC_LIST ./muduo_server.cpp)
#设置可执行文件生成的目录
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

#aux_source_directory(. SRC_LIST)

add_executable(server ${SRC_LIST})
target_link_libraries(server muduo_net muduo_base pthread)