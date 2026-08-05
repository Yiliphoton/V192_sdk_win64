# V192 LiDAR Decoder SDK — Windows x64

[中文](#中文说明) | [English](#english)

---

## 中文说明

### 简介

V192 SDK 提供 `v192_filter.dll`，用于解码一力光子（yiliphoton）V192 192 通道激光雷达（MSOP 协议）的点云数据，并内置近邻滤波器。支持实时 UDP 接收、PCAP 离线回放和手动数据包注入三种模式。

**核心参数**
- 192 通道，每帧 153 600 点（双回波：307 200 点）
- 水平角分辨率 0.09°，800 个 slot/帧
- 坐标系：右手系，X=正前方

### 文件结构

```
V192_SDK/
├── include/
│   ├── v192_decoder.h       # C 公共 API（解码器）
│   └── v192_filter.h        # C++ API（点云近邻滤波器）
├── dll/
│   ├── Release/
│   │   ├── v192_filter.dll  # 主 DLL（Release）
│   │   ├── v192_filter.lib  # 导入库
│   │   └── V192-Correction.json
│   ├── Debug/
│   │   ├── v192_filter.dll  # 主 DLL（Debug）
│   │   ├── v192_filter.lib
│   │   ├── v192_filter.pdb
│   │   └── V192-Correction.json
│   └── 说明.md
├── demo/                    # CMake C++ 示例
└── qt_demo/                 # Qt 示例（V192Viewer）
```

> `v192_decoder` 是内部 OBJECT library，已静态编译进 `v192_filter.dll`，不存在单独的 `v192_decoder.dll`。

### 前提条件

| 依赖 | 说明 |
|------|------|
| Windows 10/11 x64 | 仅支持 64 位 |
| Visual Studio 2019/2022 | MSVC 工具链，C++14 |
| CMake 3.10+ | 用于构建 demo |
| [Npcap](https://npcap.com/) | 在线模式需要在目标机器上安装 |

### 快速集成

#### CMake 项目

```cmake
target_include_directories(my_app PRIVATE path/to/V192_SDK/include)

target_link_libraries(my_app PRIVATE
    "$<$<CONFIG:Debug>:path/to/V192_SDK/dll/Debug/v192_filter.lib>"
    "$<$<CONFIG:Release>:path/to/V192_SDK/dll/Release/v192_filter.lib>"
)
```

构建后将 `dll/<Config>/v192_filter.dll` 复制到可执行文件目录，或使用 `add_custom_command` 自动复制。

#### Visual Studio 项目

1. **C/C++ → 附加包含目录**：添加 `include\`
2. **链接器 → 附加依赖项**：添加 `dll\Release\v192_filter.lib`
3. 将 `v192_filter.dll` 和 `V192-Correction.json` 复制到输出目录

### 快速上手

#### 在线模式（实时 UDP 接收）

```cpp
#include "v192_decoder.h"

void on_frame(const V192Frame* frame, void* ud) {
    printf("Frame %u: %u points\n", frame->frame_id, frame->count);
    for (uint32_t i = 0; i < frame->count; i++) {
        const V192Point* p = &frame->points[i];
        // p->x, p->y, p->z (m)  p->distance  p->azimuth  p->elevation
        // p->reflectivity (0-255)  p->ring (0-191)  p->timestamp
    }
}

int main() {
    V192DecoderHandle h = v192_create("V192-Correction.json"); // NULL 使用内置默认值
    v192_set_frame_callback(h, on_frame, nullptr);
    v192_set_distance_range(h, 0.5f, 150.0f);  // 可选：距离过滤
    v192_start_online(h, 8899);                 // 需要管理员权限 + Npcap
    // ... 等待 / 事件循环 ...
    v192_stop(h);
    v192_destroy(h);
}
```

#### 离线模式（PCAP 文件）

```cpp
V192DecoderHandle h = v192_create("V192-Correction.json");
v192_set_frame_callback(h, on_frame, nullptr);
v192_open_pcap(h, "capture.pcap");
while (v192_read_next_frame(h) == 0) {}
v192_close_pcap(h);
v192_destroy(h);
```

#### 点云滤波器（可选）

```cpp
#include "v192_filter.h"

V192FilterParam param;
param.close_dis_thres = 30;  // 近距离过滤阈值（cm）

V192PointCloudFilter filter(param);
filter.install(h);   // 挂载到解码器管线（在 start/open 之前调用）
// ... 正常接收 ...
filter.uninstall(h);
```

### API 速查

| 函数 | 说明 |
|------|------|
| `v192_create(calib)` | 创建解码器；`calib` 为 NULL 时使用内置角度 |
| `v192_destroy(h)` | 销毁实例，释放所有资源 |
| `v192_set_frame_callback(h, cb, ud)` | 注册帧回调（start/open 之前调用） |
| `v192_start_online(h, port)` | 启动 UDP 接收线程（默认端口 8899） |
| `v192_stop(h)` | 停止接收线程 |
| `v192_open_pcap(h, path)` | 打开 PCAP 文件 |
| `v192_read_next_frame(h)` | 解码下一帧（0=成功，1=EOF，-1=错误） |
| `v192_close_pcap(h)` | 关闭 PCAP 文件 |
| `v192_feed_packet(h, data, len)` | 手动注入原始 UDP 负载 |
| `v192_set_distance_range(h, min, max)` | 设置距离过滤范围（米） |
| `v192_set_filter(h, cb, ud)` | 挂载自定义滤波回调；传 NULL 移除 |

### 部署清单

随应用程序分发时须包含：

- `v192_filter.dll`
- `V192-Correction.json`（建议放在 `.exe` 同目录）
- Npcap 运行时（在线模式的目标机器需安装）

### 常见问题

**找不到 `v192_filter.dll`** — DLL 须与 `.exe` 在同一目录，或在 `PATH` 中。

**在线模式收不到数据** — 以管理员权限运行；确认 Npcap 已安装；防火墙放行 UDP 端口（默认 8899）。

**所有点坐标为 NaN** — 标定文件路径错误，或 `v192_create()` 返回 NULL（查看控制台输出）。

**链接错误 `__imp_v192_create`** — 须链接 `v192_filter.lib`，不要链接 `v192_decoder.lib`（该文件不存在）。

---

## English

### Overview

The V192 SDK ships `v192_filter.dll`, a Windows x64 library for decoding point-cloud data from the yiliphoton (一力光子) V192 192-channel LiDAR (MSOP protocol). It bundles the decoder and a neighbour-based point-cloud filter in a single DLL.

**Key specs**
- 192 channels, 153,600 points/frame (307,200 in dual-echo mode)
- 0.09° horizontal resolution, 800 slots/frame
- Right-hand coordinate system: X = forward

### File Layout

```
V192_SDK/
├── include/
│   ├── v192_decoder.h       # C public API (decoder)
│   └── v192_filter.h        # C++ API (point-cloud filter)
├── dll/
│   ├── Release/
│   │   ├── v192_filter.dll  # Main DLL (Release)
│   │   ├── v192_filter.lib  # Import library
│   │   └── V192-Correction.json
│   ├── Debug/
│   │   ├── v192_filter.dll  # Main DLL (Debug)
│   │   ├── v192_filter.lib
│   │   ├── v192_filter.pdb
│   │   └── V192-Correction.json
│   └── 说明.md
├── demo/                    # CMake C++ sample
└── qt_demo/                 # Qt sample application (V192Viewer)
```

> The `v192_decoder` component is compiled as an OBJECT library and statically linked into `v192_filter.dll`. There is no separate `v192_decoder.dll`.

### Prerequisites

| Dependency | Notes |
|------------|-------|
| Windows 10/11 x64 | 64-bit only |
| Visual Studio 2019/2022 | MSVC toolchain, C++14 |
| CMake 3.10+ | Required to build the demo |
| [Npcap](https://npcap.com/) | Must be installed on machines using online (live) mode |

### Integration

#### CMake

```cmake
target_include_directories(my_app PRIVATE path/to/V192_SDK/include)

target_link_libraries(my_app PRIVATE
    "$<$<CONFIG:Debug>:path/to/V192_SDK/dll/Debug/v192_filter.lib>"
    "$<$<CONFIG:Release>:path/to/V192_SDK/dll/Release/v192_filter.lib>"
)
```

Copy `dll/<Config>/v192_filter.dll` next to the executable after the build, or use `add_custom_command` to automate it (see `demo/CMakeLists.txt` for a working example).

#### Visual Studio

1. **C/C++ → Additional Include Directories**: add `include\`
2. **Linker → Additional Dependencies**: add `dll\Release\v192_filter.lib`
3. Copy `v192_filter.dll` and `V192-Correction.json` to the output directory

### Quick Start

#### Online mode (live UDP)

```cpp
#include "v192_decoder.h"

void on_frame(const V192Frame* frame, void* ud) {
    printf("Frame %u: %u points\n", frame->frame_id, frame->count);
    for (uint32_t i = 0; i < frame->count; i++) {
        const V192Point* p = &frame->points[i];
        // p->x, p->y, p->z  (metres, right-hand: X=forward)
        // p->distance        (radial distance, metres)
        // p->azimuth         (degrees)  p->elevation (degrees)
        // p->reflectivity    (0-255)    p->ring (0-191)
        // p->timestamp       (seconds since Unix epoch)
    }
}

int main() {
    V192DecoderHandle h = v192_create("V192-Correction.json"); // NULL = built-in defaults
    v192_set_frame_callback(h, on_frame, nullptr);
    v192_set_distance_range(h, 0.5f, 150.0f); // optional distance filter
    v192_start_online(h, 8899);               // requires admin + Npcap
    // ... wait / event loop ...
    v192_stop(h);
    v192_destroy(h);
}
```

#### PCAP (offline) mode

```cpp
V192DecoderHandle h = v192_create("V192-Correction.json");
v192_set_frame_callback(h, on_frame, nullptr);
v192_open_pcap(h, "capture.pcap");
while (v192_read_next_frame(h) == 0) {}
v192_close_pcap(h);
v192_destroy(h);
```

#### Point-cloud filter (optional)

```cpp
#include "v192_filter.h"

V192FilterParam param;
param.close_dis_thres = 30; // close-range rejection threshold (cm)

V192PointCloudFilter filter(param);
filter.install(h);   // attach before v192_start_online / v192_open_pcap
// ... receive frames normally ...
filter.uninstall(h);
```

### API Reference

| Function | Description |
|----------|-------------|
| `v192_create(calib)` | Create decoder; pass NULL to use built-in angles |
| `v192_destroy(h)` | Destroy instance and free all resources |
| `v192_set_frame_callback(h, cb, ud)` | Register frame callback (call before start/open) |
| `v192_start_online(h, port)` | Start UDP receive thread (default port 8899) |
| `v192_stop(h)` | Stop the receive thread |
| `v192_open_pcap(h, path)` | Open a PCAP file for offline playback |
| `v192_read_next_frame(h)` | Decode one frame (0=ok, 1=EOF, -1=error) |
| `v192_close_pcap(h)` | Close the PCAP file |
| `v192_feed_packet(h, data, len)` | Feed a raw UDP payload manually |
| `v192_set_distance_range(h, min, max)` | Distance filter range in metres |
| `v192_set_filter(h, cb, ud)` | Install custom filter callback; NULL removes it |

### Deployment Checklist

Distribute these files alongside your application:

- `v192_filter.dll` — place next to the executable
- `V192-Correction.json` — place next to the executable
- Npcap runtime — must be installed on any machine using online mode

### Troubleshooting

**`v192_filter.dll` not found at startup** — the DLL must be in the same directory as the executable or on `PATH`.

**Online mode receives no data** — run as Administrator; verify Npcap is installed; check that the firewall allows UDP on the configured port (default 8899).

**All point coordinates are NaN** — the calibration file path is wrong, or `v192_create()` returned NULL (check console output).

**Linker error `__imp_v192_create`** — link against `v192_filter.lib`, not `v192_decoder.lib` (the latter does not exist as a standalone file).
