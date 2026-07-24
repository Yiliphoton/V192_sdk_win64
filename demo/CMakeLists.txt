cmake_minimum_required(VERSION 3.10)
project(v192_decoder_demo)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# v192_decoder 的路径（相对于此 CMakeLists.txt）
set(V192_DECODER_DIR "${CMAKE_CURRENT_SOURCE_DIR}/..")

# 添加头文件目录
include_directories(${V192_DECODER_DIR}/include)

# 添加库目录
link_directories(${V192_DECODER_DIR}/build/Release)

# Windows: 需要 Npcap SDK（与主项目相同的配置）
if(WIN32)
    set(NPCAP_SDK_DIR $ENV{NPCAP_SDK_DIR})
    if(NOT NPCAP_SDK_DIR)
        set(NPCAP_SDK_DIR "D:/npcap-sdk-1.16")
    endif()
    
    find_path(PCAP_INCLUDE_DIR pcap.h
        PATHS ${NPCAP_SDK_DIR}/Include
              "C:/WpdPack/Include"
    )
    
    if(CMAKE_SIZEOF_VOID_P EQUAL 8)
        find_library(PCAP_LIBRARY NAMES wpcap
            PATHS ${NPCAP_SDK_DIR}/Lib/x64
                  "C:/WpdPack/Lib/x64"
        )
        find_library(PACKET_LIBRARY NAMES Packet
            PATHS ${NPCAP_SDK_DIR}/Lib/x64
                  "C:/WpdPack/Lib/x64"
        )
    else()
        find_library(PCAP_LIBRARY NAMES wpcap
            PATHS ${NPCAP_SDK_DIR}/Lib
                  "C:/WpdPack/Lib"
        )
        find_library(PACKET_LIBRARY NAMES Packet
            PATHS ${NPCAP_SDK_DIR}/Lib
                  "C:/WpdPack/Lib"
        )
    endif()
    
    set(PLATFORM_LIBS ${PCAP_LIBRARY} ${PACKET_LIBRARY} ws2_32)
else()
    find_library(PCAP_LIBRARY NAMES pcap)
    set(PLATFORM_LIBS ${PCAP_LIBRARY})
endif()

# 创建示例程序
add_executable(simple_demo simple_demo.cpp)

# 链接 v192_decoder 和平台库
target_link_libraries(simple_demo v192_decoder ${PLATFORM_LIBS})

# 复制 DLL 到输出目录（Windows 运行时需要）
if(WIN32)
    add_custom_command(TARGET simple_demo POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${V192_DECODER_DIR}/build/Release/v192_decoder.dll"
            $<TARGET_FILE_DIR:simple_demo>
        COMMENT "Copying v192_decoder.dll to output directory"
    )
endif()

# 打印配置信息
message(STATUS "")
message(STATUS "  v192_decoder Demo")
message(STATUS "  v192_decoder path: ${V192_DECODER_DIR}")
message(STATUS "  Include directory: ${V192_DECODER_DIR}/include")
message(STATUS "  Library directory: ${V192_DECODER_DIR}/build/Release")
if(WIN32)
    message(STATUS "  Npcap/WinPcap lib: ${PCAP_LIBRARY}")
    if(PACKET_LIBRARY)
        message(STATUS "  Packet.lib       : ${PACKET_LIBRARY}")
    endif()
endif()
message(STATUS "")
