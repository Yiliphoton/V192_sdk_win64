QT       += core gui widgets

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# 项目名称
TARGET = V192Viewer
TEMPLATE = app

# 源文件
SOURCES += \
    main.cpp \
    mainwindow.cpp

# 头文件
HEADERS += \
    mainwindow.h

# UI文件
FORMS += \
    mainwindow.ui

# V192 Decoder SDK 配置
INCLUDEPATH += $$PWD

# Npcap SDK 配置
INCLUDEPATH += $$PWD/npcap-sdk-1.16/Include

msvc {
    QMAKE_CFLAGS += /utf-8
    QMAKE_CXXFLAGS += /utf-8
}
# Windows 配置
win32 {
    # 链接到 v192_filter.lib (导入库)
    LIBS += -L$$PWD -lv192_filter

    # 链接 Npcap SDK 库
    contains(QT_ARCH, x86_64) {
        LIBS += -L$$PWD/npcap-sdk-1.16/Lib/x64 -lwpcap -lPacket
    } else:contains(QT_ARCH, arm64) {
        LIBS += -L$$PWD/npcap-sdk-1.16/Lib/ARM64 -lwpcap -lPacket
    } else {
        LIBS += -L$$PWD/npcap-sdk-1.16/Lib -lwpcap -lPacket
    }
    
    # 运行时需要 v192_decoder.dll，可以在构建后复制
    # 或者将 DLL 路径添加到 PATH
}

# Linux 配置
unix:!macx {
    LIBS +=  -L$$PWD -lv192_decoder
    LIBS += -lpcap
}

# 默认部署规则
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
