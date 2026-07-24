<div align="center">

# V192 SDK (Windows x64)

Official decoder SDK for V192 3D SPAD LiDAR sensor

[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](./LICENSE)
[![Platform](https://img.shields.io/badge/Platform-Windows_x64-green.svg)]()
[![Version](https://img.shields.io/badge/DLL_Version-1.2.0-orange.svg)]()

[English](#english) | [中文](#中文)

</div>

---

## English

### Overview

V192 SDK provides a high-performance C API for decoding V192 3D SPAD LiDAR data on Windows x64 platform. It supports real-time UDP capture and offline PCAP playback, with 192-channel point cloud output and dual-echo capability.

### Key Specifications

| Parameter | Value |
|-----------|-------|
| Channels | 192 |
| Points per frame | ~307,200 |
| Frame rate | 10 fps |
| Dual-echo | Supported |
| Interface | UDP (default port 8899) |

### Features

- Real-time 3D point cloud decoding via live UDP
- Offline PCAP file parsing and playback
- Manual packet feed mode for custom integration
- Configurable distance filtering
- Per-channel calibration correction (gain/offset)
- Runtime statistics (frame count, error count, FPS)
- Thread-safe callback mechanism

### Requirements

| Item | Requirement |
|------|-------------|
| OS | Windows 10/11 (64-bit) |
| Compiler | MSVC 2019+ |
| Network | [Npcap](https://npcap.com/) (for online capture) |
| Runtime | Visual C++ Redistributable 2019+ |

### Directory Structure

```
V192_sdk_win64/
├── dll/               # v192_decoder.dll, .lib, and calibration file
│   ├── Release/       # Production build
│   └── Debug/         # Debug build
├── demo/              # Command-line demo (C/C++)
│   └── simple_demo.cpp
├── qt_demo/           # Qt-based point cloud viewer
│   ├── mainwindow.cpp
│   ├── V192Viewer.pro
│   └── ...
├── docs/              # Additional documentation
├── examples/          # Integration examples
├── CHANGELOG.md       # Version history
└── LICENSE            # BSD 3-Clause
```

### Quick Start

#### 1. Integration

```cpp
#include "v192_decoder.h"

// Create decoder (NULL for built-in calibration)
V192DecoderHandle decoder = v192_create(NULL);

// Register frame callback
v192_set_frame_callback(decoder, on_frame_ready, NULL);

// Set distance filter (optional)
v192_set_distance_range(decoder, 0.1f, 200.0f);
```

#### 2. Online Mode (Live UDP)

```cpp
// Start receiving on port 8899
v192_start_online(decoder, 8899);

// ... data arrives via callback ...

// Stop when done
v192_stop(decoder);
v192_destroy(decoder);
```

#### 3. Offline Mode (PCAP)

```cpp
// Open PCAP file
v192_open_pcap(decoder, "capture.pcap");

// Read frames one by one (callback invoked per frame)
while (v192_read_next_frame(decoder) == 0) {
    // Processing happens in callback
}

v192_close_pcap(decoder);
v192_destroy(decoder);
```

#### 4. Frame Callback

```cpp
void on_frame_ready(const V192Frame* frame, void* user_data) {
    printf("Frame #%u: %u points\n", frame->frame_id, frame->count);

    for (uint32_t i = 0; i < frame->count; i++) {
        const V192Point* pt = &frame->points[i];
        // Access: pt->x, pt->y, pt->z, pt->distance,
        //         pt->azimuth, pt->elevation,
        //         pt->reflectivity, pt->ring
    }
}
```

### API Reference

| Function | Description |
|----------|-------------|
| `v192_create()` | Create decoder instance |
| `v192_destroy()` | Destroy and free resources |
| `v192_set_frame_callback()` | Register frame-ready callback |
| `v192_start_online()` | Start live UDP capture |
| `v192_stop()` | Stop live capture |
| `v192_open_pcap()` | Open PCAP file for offline decoding |
| `v192_read_next_frame()` | Decode next frame from PCAP |
| `v192_close_pcap()` | Close PCAP file |
| `v192_feed_packet()` | Feed raw UDP packet manually |
| `v192_set_distance_range()` | Set min/max distance filter |

See [dll/说明.md](./dll/说明.md) for full API documentation.

### Demos

| Demo | Description | Build |
|------|-------------|-------|
| [demo/](./demo/) | Command-line demo — online capture & PCAP playback | CMake or manual `cl` |
| [qt_demo/](./qt_demo/) | Qt GUI viewer — real-time display, statistics, PCD/PLY export | Qt Creator or qmake |

### Point Cloud Export Formats

The Qt demo supports exporting point cloud data:

| Format | Extension | Compatible With |
|--------|-----------|-----------------|
| TXT | `.txt` | Any text editor |
| PCD | `.pcd` | PCL, CloudCompare |
| PLY | `.ply` | MeshLab, CloudCompare, Open3D |

### Support

- Website: [www.yiliphoton.com](https://www.yiliphoton.com)
- Email: sales@yiliphoton.com
- Issues: [GitHub Issues](https://github.com/Yiliphoton/V192_sdk_win64/issues)

### License

BSD 3-Clause License. See [LICENSE](./LICENSE).

---

## 中文

### 概述

V192 SDK 为 V192 3D SPAD 激光雷达提供高性能 Windows x64 解码库（C API），支持实时 UDP 采集与离线 PCAP 回放，输出 192 通道点云数据，支持双回波模式。

### 主要参数

| 参数 | 数值 |
|------|------|
| 通道数 | 192 |
| 每帧点数 | ~307,200 |
| 帧率 | 10 fps |
| 双回波 | 支持 |
| 通信接口 | UDP（默认端口 8899） |

### 功能特性

- 实时 UDP 3D 点云解码
- 离线 PCAP 文件解析与回放
- 手动数据包注入模式（自定义集成）
- 可配置距离过滤
- 逐通道增益/偏置校正
- 运行时统计（帧数、错误帧数、帧率）
- 线程安全回调机制

### 系统要求

| 项目 | 要求 |
|------|------|
| 操作系统 | Windows 10/11（64 位） |
| 编译器 | MSVC 2019 及以上 |
| 网络驱动 | [Npcap](https://npcap.com/)（在线采集需要） |
| 运行时 | Visual C++ Redistributable 2019+ |

### 目录结构

```
V192_sdk_win64/
├── dll/               # v192_decoder.dll、.lib 及校正文件
│   ├── Release/       # 生产环境版本
│   └── Debug/         # 调试版本
├── demo/              # 命令行示例（C/C++）
│   └── simple_demo.cpp
├── qt_demo/           # Qt 点云查看器
│   ├── mainwindow.cpp
│   ├── V192Viewer.pro
│   └── ...
├── docs/              # 附加文档
├── examples/          # 集成示例
├── CHANGELOG.md       # 版本更新记录
└── LICENSE            # BSD 3-Clause 许可证
```

### 快速开始

#### 1. 集成

```cpp
#include "v192_decoder.h"

// 创建解码器（NULL 使用内置校正参数）
V192DecoderHandle decoder = v192_create(NULL);

// 注册帧回调
v192_set_frame_callback(decoder, on_frame_ready, NULL);

// 设置距离过滤（可选）
v192_set_distance_range(decoder, 0.1f, 200.0f);
```

#### 2. 在线模式（实时 UDP）

```cpp
// 在端口 8899 上启动接收
v192_start_online(decoder, 8899);

// ... 数据通过回调函数返回 ...

// 完成后停止
v192_stop(decoder);
v192_destroy(decoder);
```

#### 3. 离线模式（PCAP 文件）

```cpp
// 打开 PCAP 文件
v192_open_pcap(decoder, "capture.pcap");

// 逐帧读取（每帧解码完成后触发回调）
while (v192_read_next_frame(decoder) == 0) {
    // 数据在回调函数中处理
}

v192_close_pcap(decoder);
v192_destroy(decoder);
```

#### 4. 帧回调函数

```cpp
void on_frame_ready(const V192Frame* frame, void* user_data) {
    printf("帧 #%u: %u 个点\n", frame->frame_id, frame->count);

    for (uint32_t i = 0; i < frame->count; i++) {
        const V192Point* pt = &frame->points[i];
        // 可访问: pt->x, pt->y, pt->z, pt->distance,
        //         pt->azimuth, pt->elevation,
        //         pt->reflectivity, pt->ring
    }
}
```

### API 一览

| 函数 | 说明 |
|------|------|
| `v192_create()` | 创建解码器实例 |
| `v192_destroy()` | 销毁并释放资源 |
| `v192_set_frame_callback()` | 注册帧回调函数 |
| `v192_start_online()` | 启动实时 UDP 采集 |
| `v192_stop()` | 停止实时采集 |
| `v192_open_pcap()` | 打开 PCAP 文件 |
| `v192_read_next_frame()` | 解码下一帧 |
| `v192_close_pcap()` | 关闭 PCAP 文件 |
| `v192_feed_packet()` | 手动注入原始 UDP 数据包 |
| `v192_set_distance_range()` | 设置距离过滤范围 |

完整 API 文档请参阅 [dll/说明.md](./dll/说明.md)。

### 示例程序

| 示例 | 说明 | 构建方式 |
|------|------|----------|
| [demo/](./demo/) | 命令行示例 — 在线采集与 PCAP 回放 | CMake 或手动 `cl` 编译 |
| [qt_demo/](./qt_demo/) | Qt GUI 查看器 — 实时显示、统计信息、PCD/PLY 导出 | Qt Creator 或 qmake |

### 点云导出格式

Qt 示例程序支持导出以下格式：

| 格式 | 扩展名 | 兼容软件 |
|------|--------|----------|
| TXT | `.txt` | 任意文本编辑器 |
| PCD | `.pcd` | PCL、CloudCompare |
| PLY | `.ply` | MeshLab、CloudCompare、Open3D |

### 技术支持

- 官网：[www.yiliphoton.com](https://www.yiliphoton.com)
- 邮箱：sales@yiliphoton.com
- Issues：[GitHub Issues](https://github.com/Yiliphoton/V192_sdk_win64/issues)

### 许可证

BSD 3-Clause 许可证。详见 [LICENSE](./LICENSE)。

---

<div align="center">
  <b>Yiliphoton | 杭州一力光子科技有限公司</b><br>
  3D SPAD Digital LiDAR Solutions<br>
  <a href="https://www.yiliphoton.com">www.yiliphoton.com</a>
</div>
