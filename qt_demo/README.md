# V192 点云查看器 - Qt示例

这是一个使用Qt框架调用V192 Decoder DLL的完整示例应用程序。

## 功能特性

### 核心功能
- ✅ **在线模式**: 通过UDP实时接收V192激光雷达数据
- ✅ **离线模式**: 读取和回放PCAP文件
- ✅ **实时统计**: 显示帧率、点数、距离范围等信息
- ✅ **点详情**: 显示前10个有效点的详细信息
- ✅ **距离过滤**: 可配置最小/最大距离范围
- ✅ **线程安全**: 使用Qt信号槽机制处理跨线程数据传递

### 数据导出功能 (v1.1.0新增)
- ✅ **时间戳文件夹**: 自动创建以当前时间命名的文件夹保存数据
- ✅ **自动保存**: 实时保存每一帧为TXT文件
- ✅ **PCD导出**: 导出为PCL兼容的PCD格式
- ✅ **PLY导出**: 导出为通用的PLY格式
- ✅ **手动保存**: 随时保存当前帧

## 项目结构

```
qt_example/
├── main.cpp              # 应用程序入口
├── mainwindow.h          # 主窗口头文件
├── mainwindow.cpp        # 主窗口实现
├── mainwindow.ui         # Qt Designer UI文件
├── V192Viewer.pro        # Qt项目文件
└── README.md             # 本文件
```

## 环境要求

### 必需软件
- **Qt 5.12+** 或 **Qt 6.x**
- **Qt Creator** (推荐) 或 **qmake**
- **MSVC 2019+** (Windows) 或 **GCC 7+** (Linux)
- **V192 Decoder SDK** (已编译的DLL/SO库)

### Windows环境
```bash
# 确保已安装Qt
# 下载地址: https://www.qt.io/download

# 确保已编译V192 Decoder
cd ../build
cmake --build . --config Release
```

### Linux环境
```bash
# 安装Qt开发包
sudo apt-get install qt5-default qtcreator

# 安装libpcap开发包
sudo apt-get install libpcap-dev

# 编译V192 Decoder
cd ../build
cmake .. && make -j4
```

## 编译方法

### 方法1: 使用Qt Creator (推荐)

1. 打开Qt Creator
2. 选择 `文件` → `打开文件或项目`
3. 选择 `V192Viewer.pro`
4. 配置构建套件 (Kit)
5. 点击 `构建` → `构建项目`
6. 点击 `运行` 按钮启动应用

### 方法2: 使用命令行 (qmake)

#### Windows
```cmd
cd qt_example
qmake V192Viewer.pro
nmake release

REM 复制DLL到可执行文件目录
copy ..\build\Release\v192_decoder.dll release\
copy ..\build\Release\V192-Correction.json release\

REM 运行
release\V192Viewer.exe
```

#### Linux
```bash
cd qt_example
qmake V192Viewer.pro
make -j4

# 复制库文件
cp ../build/libv192_decoder.so .
cp ../build/V192-Correction.json .

# 运行
./V192Viewer
```

### 方法3: 使用CMake (可选)

如果你更喜欢CMake，可以创建 `CMakeLists.txt`:

```cmake
cmake_minimum_required(VERSION 3.16)
project(V192Viewer)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5 REQUIRED COMPONENTS Core Widgets)

add_executable(V192Viewer
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

target_include_directories(V192Viewer PRIVATE ../include)
target_link_libraries(V192Viewer 
    Qt5::Core 
    Qt5::Widgets
    ${CMAKE_SOURCE_DIR}/../build/Release/v192_decoder.lib
)
```

## 使用说明

### 在线模式（实时接收）

1. 确保V192激光雷达已连接并配置为发送数据到本机
2. 在 **UDP端口** 输入框中输入端口号（默认8899）
3. 设置 **最小距离** 和 **最大距离** 过滤范围
4. （可选）勾选 **自动保存到时间戳文件夹** 启用自动保存
5. 点击 **启动在线接收** 按钮
6. 应用程序将开始接收并显示点云数据
7. 点击 **停止接收** 按钮停止

### 离线模式（PCAP回放）

1. 点击 **打开PCAP文件** 按钮
2. 选择一个包含V192数据的PCAP文件
3. 点击 **读取下一帧** 按钮逐帧查看数据
4. 点击 **关闭PCAP** 按钮关闭文件

### 数据导出功能

#### 自动保存
1. 勾选 **自动保存到时间戳文件夹** 复选框
2. 程序自动创建时间戳文件夹（如：`20260522_164530`）
3. 每接收一帧自动保存为TXT文件
4. 文件命名：`frame_00001.txt`, `frame_00002.txt` 等

#### 手动保存当前帧
1. 接收或读取数据后
2. 点击 **保存当前帧(TXT)** 按钮
3. 数据保存到时间戳文件夹

#### 导出点云格式
1. 确保有当前帧数据
2. 点击 **导出PCD** 或 **导出PLY** 按钮
3. 选择保存位置和文件名
4. 使用CloudCompare、MeshLab或PCL工具打开查看

**支持的导出格式**:
- **TXT**: 文本格式，包含完整点信息（ring, distance, azimuth, elevation, xyz, reflectivity）
- **PCD**: Point Cloud Data格式，兼容PCL库
- **PLY**: Polygon File Format，通用3D格式

### 界面说明

#### 统计信息面板
- **帧ID**: 当前帧的序列号
- **总帧数**: 已接收的总帧数
- **点数**: 当前帧的总点数
- **有效点数**: 距离大于0.01m的点数
- **帧率(FPS)**: 实时计算的帧率
- **双回波**: 是否包含双回波数据
- **距离范围**: 当前帧中点的最小和最大距离

#### 点详情面板
显示前10个有效点的详细信息：
- Ring: 通道编号 (0-191)
- dist: 径向距离 (米)
- az: 方位角 (度)
- el: 俯仰角 (度)
- xyz: 笛卡尔坐标 (米)
- refl: 反射强度 (0-255)

## 代码架构

### 线程模型

```
┌─────────────────┐
│  UDP接收线程    │ (v192_decoder内部)
│  (C++ SDK)      │
└────────┬────────┘
         │ 回调
         ▼
┌─────────────────┐
│  frameCallback  │ (静态C函数)
│  快速复制数据   │
└────────┬────────┘
         │ 互斥锁
         ▼
┌─────────────────┐
│  m_latestFrame  │ (共享数据)
│  (QMutex保护)   │
└────────┬────────┘
         │ 定时器轮询
         ▼
┌─────────────────┐
│  Qt主线程       │
│  更新UI         │
└─────────────────┘
```

### 关键类和方法

#### MainWindow类
- `initDecoder()`: 初始化V192解码器
- `frameCallback()`: 静态回调函数，接收解码后的帧数据
- `onUpdateUI()`: 定时器触发，更新UI显示
- `updateStatistics()`: 更新统计信息

#### 线程安全机制
- 使用 `QMutex` 保护共享的帧数据
- 回调函数中只做快速复制，不阻塞UDP接收
- UI更新在主线程中通过定时器轮询完成

## 扩展开发

### 添加3D可视化

可以集成Qt 3D或VTK来显示点云：

```cpp
// 示例：使用Qt 3D显示点云
#include <Qt3DCore>
#include <Qt3DRender>

void MainWindow::visualizePointCloud(const FrameData& frame) {
    // 创建点云几何体
    // 更新3D视图
}
```

### 添加数据导出

```cpp
void MainWindow::exportToPCD(const FrameData& frame, const QString& filename) {
    QFile file(filename);
    if (file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&file);
        out << "# .PCD v0.7 - Point Cloud Data file format\n";
        out << "VERSION 0.7\n";
        out << "FIELDS x y z intensity\n";
        // ... 写入点云数据
    }
}
```

### 添加ROS集成

```cpp
// 发布到ROS话题
#include <ros/ros.h>
#include <sensor_msgs/PointCloud2.h>

void MainWindow::publishToROS(const FrameData& frame) {
    sensor_msgs::PointCloud2 msg;
    // 转换并发布
    m_rosPublisher.publish(msg);
}
```

## 常见问题

### Q: 找不到v192_decoder.dll
**A**: 确保DLL文件在以下位置之一：
- 与可执行文件同目录
- 系统PATH环境变量包含的目录
- 或者在.pro文件中添加：
  ```qmake
  QMAKE_POST_LINK += $$quote(copy /Y \"$$PWD\\..\\build\\Release\\v192_decoder.dll\" \"$$OUT_PWD\\release\")
  ```

### Q: 编译错误：找不到Qt头文件
**A**: 确保Qt安装正确，并在Qt Creator中配置了正确的Kit

### Q: 运行时崩溃
**A**: 检查：
1. DLL版本是否匹配
2. 是否正确设置了回调函数
3. 是否在正确的线程中访问Qt对象

### Q: 帧率很低或卡顿
**A**: 
1. 减少UI更新频率（增加定时器间隔）
2. 在点详情面板中显示更少的点
3. 考虑使用独立的业务线程处理数据

## 性能优化建议

1. **减少UI更新频率**: 将定时器间隔从100ms增加到200ms或更多
2. **使用对象池**: 重用FrameData对象，避免频繁分配内存
3. **异步处理**: 将数据保存、导出等耗时操作放到独立线程
4. **批量更新**: 累积多帧数据后再更新UI

## 许可证

本示例代码遵循与V192 Decoder SDK相同的许可证。

## 技术支持

如有问题，请参考：
- V192 Decoder SDK文档: `../README.md`
- Qt官方文档: https://doc.qt.io/
- 项目Issues页面

## 更新日志

### v1.1.0 (2026-05-22)
- ✅ 新增时间戳文件夹自动保存功能
- ✅ 新增PCD格式导出
- ✅ 新增PLY格式导出
- ✅ 新增手动保存当前帧功能
- ✅ 改进UI布局，新增数据导出分组
- ✅ 详细更新说明见 [CHANGELOG.md](CHANGELOG.md)

### v1.0.0 (2026-05-22)
- ✅ 初始版本
- ✅ 支持在线和离线模式
- ✅ 实时统计显示
- ✅ 线程安全的数据传递
