# v192_decoder DLL 使用示例

这是一个简单的示例项目，展示如何在您的应用程序中使用 `v192_decoder.dll`。

## 📋 前提条件

1. 已构建 `v192_decoder.dll`（位于 `../build/Release/`）
2. Windows 系统已安装 [Npcap](https://npcap.com/)（用于在线捕获）
3. CMake 3.10 或更高版本
4. Visual Studio 2019 或更高版本

## 🔨 构建示例

### 方法 1: 使用 CMake（推荐）

```cmd
cd demo
mkdir build
cd build
cmake .. -G "Visual Studio 16 2019" -A x64
cmake --build . --config Release
```

构建完成后，可执行文件位于：`build/Release/simple_demo.exe`

### 方法 2: 手动编译

```cmd
cd demo
cl /EHsc /std:c++17 /I"..\include" simple_demo.cpp /link "..\build\Release\v192_decoder.lib"
```

然后将 `v192_decoder.dll` 复制到当前目录。

## 🚀 运行示例

### 在线捕获模式（从网卡）

使用默认端口 8899：
```cmd
simple_demo.exe online
```

使用自定义端口：
```cmd
simple_demo.exe online 8899
```

**注意**：在线模式需要管理员权限！

### 离线解析模式（从 PCAP 文件）

```cmd
simple_demo.exe pcap path\to\your\file.pcap
```

例如：
```cmd
simple_demo.exe pcap ..\data\sample.pcap
```

## 📊 示例输出

```
启动在线捕获模式...
端口: 8899
设备: 自动选择

等待数据... (按 Ctrl+C 停止)

[Frame     1] points=307200  timestamp=1779435379.104  dual_return=yes
  [ 0] dist= 7.635m  az=  13.72°  el= -12.48°  xyz=(  1.769,  -7.250,  -1.615)  refl= 14  ring= 0
  [ 1] dist= 7.560m  az=  13.72°  el= -12.35°  xyz=(  1.750,  -7.175,  -1.617)  refl= 14  ring= 1
  [ 2] dist= 7.635m  az=  13.72°  el= -12.22°  xyz=(  1.769,  -7.250,  -1.615)  refl= 14  ring= 2
  [ 3] dist= 7.655m  az=  13.72°  el= -12.09°  xyz=(  1.774,  -7.272,  -1.602)  refl= 15  ring= 3
  [ 4] dist= 7.485m  az=  13.72°  el= -11.96°  xyz=(  1.736,  -7.114,  -1.550)  refl= 20  ring= 4

[Frame     2] points=307200  timestamp=1779435379.204  dual_return=yes
  ...

--- Statistics ---
  Total frames: 10
  Total points: 3072000
  Avg points/frame: 307200
  Duration: 1.00 seconds
  Frame rate: 10.0 fps
------------------

=== 最终统计 ===
总帧数: 100
总点数: 30720000
平均点数/帧: 307200
总时长: 10.00 秒
平均帧率: 10.0 fps
================
```

## 📝 代码说明

### 核心代码结构

```cpp
// 1. 创建解码器
v192::Decoder decoder;

// 2. 设置回调函数
decoder.set_callback(on_point_cloud_received, nullptr);

// 3. 启动解码器（在线或离线）
decoder.start_online(nullptr, 2368);  // 在线模式
// 或
decoder.start_pcap("file.pcap");      // 离线模式

// 4. 运行解码器（阻塞）
decoder.run();
```

### 回调函数

```cpp
void on_point_cloud_received(const v192::PointCloud& cloud, void* user_data) {
    // 处理点云数据
    for (const auto& pt : cloud.points) {
        // 访问点的属性：pt.x, pt.y, pt.z, pt.distance, etc.
    }
}
```

## 🔧 自定义修改

您可以修改 `simple_demo.cpp` 来实现自己的功能：

1. **点云过滤**：在回调函数中过滤特定距离或角度的点
2. **数据保存**：将点云保存为 PCD、PLY 或其他格式
3. **实时可视化**：集成 PCL、Open3D 等库进行可视化
4. **多线程处理**：使用队列实现生产者-消费者模式

## ⚠️ 常见问题

### 问题 1: 找不到 DLL

**错误**：`无法启动此程序，因为计算机中丢失 v192_decoder.dll`

**解决**：
- CMake 构建会自动复制 DLL
- 手动编译需要将 DLL 复制到可执行文件目录

### 问题 2: 在线捕获失败

**错误**：`错误: 无法启动在线捕获`

**解决**：
1. 以管理员权限运行
2. 确保已安装 Npcap
3. 检查雷达是否连接并发送数据

### 问题 3: PCAP 文件打开失败

**错误**：`错误: 无法打开 PCAP 文件`

**解决**：
1. 检查文件路径是否正确
2. 确认文件格式为标准 PCAP
3. 检查文件读取权限

## 📚 更多资源

- [DLL_USAGE_DEMO.md](../DLL_USAGE_DEMO.md) - 详细的 API 文档和更多示例
- [BUILD_WINDOWS.md](../BUILD_WINDOWS.md) - Windows 构建指南
- [example/](../example/) - 官方示例代码

## 📞 技术支持

如有问题，请参考项目主目录的文档或联系技术支持。

---

**版本**: 1.0.0  
**更新日期**: 2026-05-22
