cmake_minimum_required(VERSION 3.16)
project(V192Viewer LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Qt配置
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

# 查找Qt5或Qt6
find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Core Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Core Widgets)

# 源文件
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# 创建可执行文件
if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(V192Viewer
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
    )
else()
    add_executable(V192Viewer
        ${PROJECT_SOURCES}
    )
endif()

# 包含V192 Decoder SDK头文件
target_include_directories(V192Viewer PRIVATE 
    ${CMAKE_CURRENT_SOURCE_DIR}/../include
)

# 链接Qt库
target_link_libraries(V192Viewer PRIVATE 
    Qt${QT_VERSION_MAJOR}::Core
    Qt${QT_VERSION_MAJOR}::Widgets
)

# 链接V192 Decoder库
if(WIN32)
    # Windows: 链接导入库
    target_link_libraries(V192Viewer PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/Release/v192_decoder.lib
    )
    
    # 复制DLL到输出目录
    add_custom_command(TARGET V192Viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/../build/Release/v192_decoder.dll"
            $<TARGET_FILE_DIR:V192Viewer>
        COMMENT "Copying v192_decoder.dll to output directory"
    )
    
    # 复制校准文件
    add_custom_command(TARGET V192Viewer POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            "${CMAKE_CURRENT_SOURCE_DIR}/../build/Release/V192-Correction.json"
            $<TARGET_FILE_DIR:V192Viewer>
        COMMENT "Copying V192-Correction.json to output directory"
    )
else()
    # Linux: 链接共享库
    target_link_libraries(V192Viewer PRIVATE 
        ${CMAKE_CURRENT_SOURCE_DIR}/../build/libv192_decoder.so
    )
    
    # 设置RPATH
    set_target_properties(V192Viewer PROPERTIES
        INSTALL_RPATH "${CMAKE_CURRENT_SOURCE_DIR}/../build"
        BUILD_WITH_INSTALL_RPATH TRUE
    )
endif()

# Qt6需要finalize
if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(V192Viewer)
endif()

# 安装规则
install(TARGETS V192Viewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
