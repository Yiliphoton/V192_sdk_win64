@echo off
REM V192Viewer Qt示例 - Windows构建脚本

echo ========================================
echo V192 点云查看器 - 构建脚本
echo ========================================
echo.

REM 检查Qt环境
where qmake >nul 2>&1
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 未找到qmake，请确保Qt已安装并添加到PATH
    echo.
    echo 请设置Qt环境，例如：
    echo set PATH=C:\Qt\5.15.2\msvc2019_64\bin;%%PATH%%
    echo.
    pause
    exit /b 1
)

echo [1/4] 检测Qt版本...
qmake -v
echo.

REM 检查V192 Decoder DLL
if not exist "..\build\Release\v192_decoder.dll" (
    echo [错误] 未找到v192_decoder.dll
    echo 请先编译V192 Decoder SDK:
    echo   cd ..\build
    echo   cmake --build . --config Release
    echo.
    pause
    exit /b 1
)

echo [2/4] 生成Makefile...
qmake V192Viewer.pro
if %ERRORLEVEL% NEQ 0 (
    echo [错误] qmake失败
    pause
    exit /b 1
)
echo.

echo [3/4] 编译项目...
nmake release
if %ERRORLEVEL% NEQ 0 (
    echo [错误] 编译失败
    pause
    exit /b 1
)
echo.

echo [4/4] 复制依赖文件...
copy /Y "..\build\Release\v192_decoder.dll" "release\" >nul
copy /Y "..\build\Release\V192-Correction.json" "release\" >nul
echo.

echo ========================================
echo 构建成功！
echo ========================================
echo.
echo 可执行文件位置: release\V192Viewer.exe
echo.
echo 运行程序:
echo   cd release
echo   V192Viewer.exe
echo.
pause
