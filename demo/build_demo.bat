@echo off
chcp 65001 >nul
echo ============================================
echo  V192 Demo 编译脚本
echo ============================================
echo.

:: 查找 cmake.exe
where cmake >nul 2>&1
if errorlevel 1 (
    echo [错误] 未找到 cmake，请先安装 CMake 并加入 PATH
    echo   下载地址: https://cmake.org/download/
    pause
    exit /b 1
)

echo [1/3] 配置 CMake 工程...
cmake -B build -A x64
if errorlevel 1 (
    echo.
    echo [错误] CMake 配置失败
    pause
    exit /b 1
)

echo.
echo [2/3] 编译 Release...
cmake --build build --config Release
if errorlevel 1 (
    echo.
    echo [错误] 编译失败
    pause
    exit /b 1
)

echo.
echo [3/3] 完成！
echo.
echo 可执行文件: %~dp0build\Release\simple_demo.exe
echo.
echo 运行示例:
echo   build\Release\simple_demo.exe online        （在线模式，需管理员权限）
echo   build\Release\simple_demo.exe pcap xxx.pcap  （离线模式）
echo.
pause
