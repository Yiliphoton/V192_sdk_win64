# V192 Decoder SDK — Demo

`simple_demo.cpp` 演示如何通过 `v192_filter.dll` 接收和解析 V192 激光雷达点云数据。

## 前提条件

- Windows 10 / 11 x64
- Visual Studio 2019 或 2022（MSVC）
- CMake 3.10+
- [Npcap](https://npcap.com/) 运行时（在线模式需要安装）

## 编译

### 方法一：双击脚本（最简单）

直接双击 `build_demo.bat`，自动完成配置和编译。

### 方法二：命令行 CMake

**第一步：进入 demo 目录**

```bat
cd /d D:\v192_decoder\V192_SDK_win64_dll\V192_SDK\demo
```

**第二步：如果 build 目录已存在（换过路径或重新配置时），先清除缓存**

```bat
rmdir /s /q build
```

**第三步：配置并编译**

```bat
cmake -B build -A x64
cmake --build build --config Release
```

输出：`build\Release\simple_demo.exe`（`v192_filter.dll` 会自动复制过去）

编译 Debug 版本：

```bat
cmake --build build --config Debug
```

输出：`build\Debug\simple_demo.exe`

### 方法三：手动编译（无 CMake）

打开 **x64 Native Tools Command Prompt for VS 2019/2022**，执行：

```bat
cd /d D:\v192_decoder\V192_SDK_win64_dll\V192_SDK\demo
cl /EHsc /std:c++14 /I"..\include" simple_demo.cpp /link "..\dll\Release\v192_filter.lib"
copy "..\dll\Release\v192_filter.dll" .
```

> 注意：必须用 VS 的 x64 Native Tools 命令提示符，普通 CMD 没有 `cl` 命令。

## 运行

### 在线模式（实时接收）

```bat
simple_demo.exe online           # 使用默认端口 8899
simple_demo.exe online 8899      # 指定端口
```

在线模式需要以**管理员权限**运行，并确认 Npcap 已安装。按 Ctrl+C 停止。

### 离线模式（PCAP 文件）

```bat
simple_demo.exe pcap data.pcap
```

## 示例输出

```
[Frame     1] points=153600  timestamp=1779435379.104  dual_return=no
  [ 0] dist= 7.635m  az=  13.72°  el= -12.48°  xyz=(  1.769,  -7.250,  -1.615)  refl= 14  ring= 0
  [ 1] dist= 7.560m  az=  13.72°  el= -12.35°  xyz=(  1.750,  -7.175,  -1.617)  refl= 14  ring= 1
  ...

--- Statistics ---
  Total frames: 10
  Total points: 1536000
  Avg points/frame: 153600
  Duration: 1.00 seconds
  Frame rate: 10.0 fps
------------------
```

双回波模式下每帧点数为 307,200（153,600 × 2）。

## 核心代码结构

```cpp
#include "v192_decoder.h"

void on_frame(const V192Frame* frame, void* ud) {
    for (uint32_t i = 0; i < frame->count; i++) {
        const V192Point* pt = &frame->points[i];
        // pt->x, pt->y, pt->z, pt->distance, pt->reflectivity, pt->ring ...
    }
}

int main() {
    V192DecoderHandle h = v192_create("../dll/Release/V192-Correction.json");
    v192_set_frame_callback(h, on_frame, nullptr);
    v192_set_distance_range(h, 0.5f, 150.0f);

    // 在线模式
    v192_start_online(h, 8899);
    // ... 等待 ...
    v192_stop(h);

    // 或 PCAP 模式
    v192_open_pcap(h, "data.pcap");
    while (v192_read_next_frame(h) == 0) {}
    v192_close_pcap(h);

    v192_destroy(h);
}
```

## 常见问题

**找不到 v192_filter.dll** — CMake 构建会自动复制；手动编译需手动复制 `dll\Release\v192_filter.dll` 到 `.exe` 旁边。

**在线捕获失败** — 以管理员权限运行，确认 Npcap 已安装，确认雷达已连接并向本机发送 UDP 数据。

**PCAP 文件打开失败** — 检查路径是否正确，文件是否为标准 PCAP 格式。

## 参考

- `../include/v192_decoder.h` — C 公共 API
- `../include/v192_filter.h` — C++ 滤波器 API
- `../dll/说明.md` — SDK 文件清单与集成说明
