# Qt示例快速入门

## 5分钟快速开始

### Windows用户

1. **安装Qt** (如果还没有)
   - 下载Qt在线安装器: https://www.qt.io/download-qt-installer
   - 安装Qt 5.15.2 或更高版本（选择MSVC 2019 64-bit组件）

2. **设置Qt环境变量**
   ```cmd
   set PATH=C:\Qt\5.15.2\msvc2019_64\bin;%PATH%
   ```
   (根据你的Qt安装路径调整)

3. **编译V192 Decoder** (如果还没有)
   ```cmd
   cd d:\sdk\v192_decoder\build
   cmake --build . --config Release
   ```

4. **构建Qt示例**
   ```cmd
   cd d:\sdk\v192_decoder\qt_example
   build_windows.bat
   ```

5. **运行程序**
   ```cmd
   cd release
   V192Viewer.exe
   ```

### Linux用户

1. **安装Qt开发包**
   ```bash
   # Ubuntu/Debian
   sudo apt-get install qt5-default qtcreator libpcap-dev
   
   # Fedora/RHEL
   sudo dnf install qt5-qtbase-devel libpcap-devel
   ```

2. **编译V192 Decoder** (如果还没有)
   ```bash
   cd ~/v192_decoder/build
   cmake .. && make -j4
   ```

3. **构建Qt示例**
   ```bash
   cd ~/v192_decoder/qt_example
   chmod +x build_linux.sh
   ./build_linux.sh
   ```

4. **运行程序**
   ```bash
   ./V192Viewer
   ```

## 使用Qt Creator (推荐)

这是最简单的方法：

1. 打开Qt Creator
2. 文件 → 打开文件或项目
3. 选择 `qt_example/V192Viewer.pro`
4. 配置项目（选择合适的Kit）
5. 点击左下角的绿色"运行"按钮 ▶

## 测试程序

### 测试在线模式

如果你有V192激光雷达硬件：

1. 确保雷达已连接并配置为发送数据到你的PC
2. 在程序中输入UDP端口（默认8899）
3. 点击"启动在线接收"
4. 你应该能看到实时的点云数据统计

### 测试离线模式

如果你有PCAP文件：

1. 点击"打开PCAP文件"
2. 选择一个包含V192数据的.pcap文件
3. 点击"读取下一帧"逐帧查看数据

## 常见问题

### ❌ 找不到v192_decoder.dll

**解决方案**:
```cmd
REM 手动复制DLL
copy ..\build\Release\v192_decoder.dll release\
```

### ❌ 编译错误：找不到Qt

**解决方案**:
- 确保Qt已正确安装
- 将Qt的bin目录添加到PATH
- 或在Qt Creator中打开项目（会自动配置）

### ❌ 运行时错误：无法启动程序

**解决方案**:
- 确保所有Qt DLL都在PATH中
- 或使用Qt提供的windeployqt工具：
  ```cmd
  windeployqt release\V192Viewer.exe
  ```

## 下一步

- 查看 [`README.md`](README.md) 了解详细功能说明
- 查看 [`mainwindow.cpp`](mainwindow.cpp) 学习如何调用V192 Decoder API
- 修改UI文件 [`mainwindow.ui`](mainwindow.ui) 自定义界面

## 截图

程序运行后的界面包括：
- 连接设置面板（UDP端口、距离过滤）
- PCAP文件控制
- 实时统计信息（帧率、点数等）
- 点详情显示

## 技术支持

遇到问题？
1. 查看 [`README.md`](README.md) 的"常见问题"部分
2. 查看主项目的 [`README.md`](../README.md)
3. 检查Qt和编译器版本是否兼容
