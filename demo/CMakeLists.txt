cmake_minimum_required(VERSION 3.10)
project(v192_decoder_demo)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# V192 SDK 根目录（相对于本 CMakeLists.txt 的上一级）
set(V192_SDK_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

add_executable(simple_demo simple_demo.cpp)

target_include_directories(simple_demo PRIVATE
    ${V192_SDK_DIR}/include
)

# 根据构建配置自动选择 Debug 或 Release 导入库
target_link_libraries(simple_demo PRIVATE
    "$<$<CONFIG:Debug>:${V192_SDK_DIR}/dll/Debug/v192_filter.lib>"
    "$<$<CONFIG:Release>:${V192_SDK_DIR}/dll/Release/v192_filter.lib>"
)

# 构建后将对应配置的 DLL 复制到输出目录
if(WIN32)
    add_custom_command(TARGET simple_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${V192_SDK_DIR}/dll/$<CONFIG>/v192_filter.dll"
            $<TARGET_FILE_DIR:simple_demo>
        COMMENT "Copying v192_filter.dll ($<CONFIG>)"
    )
endif()
