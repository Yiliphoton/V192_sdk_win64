# Qt示例更新日志

## v1.1.0 (2026-05-22) - 新增功能

### ✨ 新增功能

#### 1. 时间戳文件夹保存功能
- **自动保存**: 新增"自动保存到时间戳文件夹"复选框
  - 启用后自动创建以当前时间命名的文件夹（格式：`YYYYMMDD_HHMMSS`）
  - 每接收一帧自动保存为TXT文件
  - 文件命名：`frame_00001.txt`, `frame_00002.txt` 等

- **手动保存**: 新增"保存当前帧(TXT)"按钮
  - 可随时保存当前显示的帧
  - 自动使用时间戳文件夹组织文件

#### 2. 点云导出功能
新增两种标准点云格式导出：

- **PCD格式导出** (Point Cloud Data)
  - 点击"导出PCD"按钮
  - 兼容PCL (Point Cloud Library)
  - 包含字段：x, y, z, intensity
  - ASCII格式，易于查看和编辑

- **PLY格式导出** (Polygon File Format)
  - 点击"导出PLY"按钮
  - 通用3D模型格式
  - 可用MeshLab、CloudCompare等工具打开
  - 包含字段：x, y, z, intensity

#### 3. 改进的UI布局
- 新增"数据导出"分组框
- 更清晰的功能分区
- 所有导出功能集中管理

### 📝 文件格式说明

#### TXT格式
```
# frame_id: 12345  timestamp: 123.456789  total_points: 36864
# ring  dist(m)  az(deg)  el(deg)  x  y  z  refl
0  12.3456  45.1234  -15.2345  8.7234  8.7234  -3.2456  128
1  15.6789  45.2345  -14.8765  11.0987  11.0987  -4.0123  156
...
```

#### PCD格式
```
# .PCD v0.7 - Point Cloud Data file format
VERSION 0.7
FIELDS x y z intensity
SIZE 4 4 4 4
TYPE F F F U
COUNT 1 1 1 1
WIDTH 36864
HEIGHT 1
VIEWPOINT 0 0 0 1 0 0 0
POINTS 36864
DATA ascii
8.723400 8.723400 -3.245600 128
11.098700 11.098700 -4.012300 156
...
```

#### PLY格式
```
ply
format ascii 1.0
comment V192 Point Cloud Data
element vertex 36864
property float x
property float y
property float z
property uchar intensity
end_header
8.723400 8.723400 -3.245600 128
11.098700 11.098700 -4.012300 156
...
```

### 🔧 代码改进

#### mainwindow.h
- 新增成员变量：
  - `QString m_outputDir` - 输出目录路径
  - `bool m_autoSave` - 自动保存标志

- 新增槽函数：
  - `onSaveFrameClicked()` - 保存当前帧
  - `onExportPCDClicked()` - 导出PCD
  - `onExportPLYClicked()` - 导出PLY
  - `onToggleAutoSaveClicked(bool)` - 切换自动保存

- 新增辅助函数：
  - `createTimestampedDirectory()` - 创建时间戳目录
  - `saveFrameToTxt()` - 保存为TXT
  - `exportToPCD()` - 导出为PCD
  - `exportToPLY()` - 导出为PLY

#### mainwindow.cpp
- 添加必要的Qt头文件：`QDateTime`, `QDir`, `QFile`, `QTextStream`
- 实现所有新增功能
- 在`onUpdateUI()`中集成自动保存逻辑

#### mainwindow.ui
- 新增"数据导出"分组框（`groupBoxExport`）
- 新增控件：
  - `chkAutoSave` - 自动保存复选框
  - `btnSaveFrame` - 保存当前帧按钮
  - `btnExportPCD` - 导出PCD按钮
  - `btnExportPLY` - 导出PLY按钮

### 📖 使用示例

#### 在线模式自动保存
```
1. 启动程序
2. 勾选"自动保存到时间戳文件夹"
3. 点击"启动在线接收"
4. 程序自动创建文件夹（如：20260522_164530）
5. 每帧自动保存为 frame_00001.txt, frame_00002.txt...
```

#### 离线模式导出点云
```
1. 点击"打开PCAP文件"
2. 点击"读取下一帧"查看数据
3. 点击"导出PCD"或"导出PLY"
4. 选择保存位置和文件名
5. 使用CloudCompare或PCL工具打开查看
```

### 🎯 应用场景

1. **数据采集**: 实时采集并自动保存所有帧数据
2. **离线分析**: 导出标准格式用于后续处理
3. **可视化**: 使用专业工具（CloudCompare, MeshLab）查看点云
4. **算法开发**: 导出数据用于SLAM、目标检测等算法测试
5. **数据归档**: 按时间自动组织数据，便于管理

### 🔄 与example_online.cpp的一致性

Qt示例的时间戳文件夹功能与命令行示例保持一致：
- 相同的时间戳格式：`YYYYMMDD_HHMMSS`
- 相同的文件命名：`frame_XXXXX.txt`
- 相同的TXT文件格式
- 跨平台支持（Windows/Linux）

### 🚀 性能优化

- 文件保存在独立线程中进行（通过Qt的事件循环）
- 不影响UDP接收性能
- 自动保存可随时开关
- 支持大量帧的连续保存

### 📦 依赖项

无新增依赖，仅使用Qt标准库：
- QtCore (QDateTime, QDir, QFile, QTextStream)
- QtWidgets (已有)

### 🐛 已知问题

无

### 📋 待实现功能

- [ ] 2D点云可视化（俯视图）
- [ ] 实时统计图表（帧率、点数曲线）
- [ ] 批量导出（导出所有帧）
- [ ] 二进制PCD格式（更小的文件）
- [ ] 点云滤波选项（距离、强度）

### 🔗 相关文档

- [README.md](README.md) - 完整功能说明
- [QUICKSTART.md](QUICKSTART.md) - 快速入门指南
- [../example/TIMESTAMP_FOLDER_FEATURE.md](../example/TIMESTAMP_FOLDER_FEATURE.md) - 命令行示例的时间戳功能

---

## v1.0.0 (2026-05-22) - 初始版本

### 基础功能
- ✅ 在线模式（UDP实时接收）
- ✅ 离线模式（PCAP回放）
- ✅ 实时统计显示
- ✅ 点详情显示
- ✅ 距离过滤
- ✅ 线程安全的数据传递
