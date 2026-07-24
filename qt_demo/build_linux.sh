#!/bin/bash
# V192Viewer Qt示例 - Linux构建脚本

echo "========================================"
echo "V192 点云查看器 - 构建脚本"
echo "========================================"
echo ""

# 检查qmake
if ! command -v qmake &> /dev/null; then
    echo "[错误] 未找到qmake，请安装Qt开发包"
    echo ""
    echo "Ubuntu/Debian:"
    echo "  sudo apt-get install qt5-default qtcreator"
    echo ""
    echo "Fedora/RHEL:"
    echo "  sudo dnf install qt5-qtbase-devel"
    echo ""
    exit 1
fi

echo "[1/4] 检测Qt版本..."
qmake -v
echo ""

# 检查V192 Decoder库
if [ ! -f "../build/libv192_decoder.so" ]; then
    echo "[错误] 未找到libv192_decoder.so"
    echo "请先编译V192 Decoder SDK:"
    echo "  cd ../build"
    echo "  cmake .. && make -j4"
    echo ""
    exit 1
fi

echo "[2/4] 生成Makefile..."
qmake V192Viewer.pro
if [ $? -ne 0 ]; then
    echo "[错误] qmake失败"
    exit 1
fi
echo ""

echo "[3/4] 编译项目..."
make -j$(nproc)
if [ $? -ne 0 ]; then
    echo "[错误] 编译失败"
    exit 1
fi
echo ""

echo "[4/4] 复制依赖文件..."
cp -f ../build/libv192_decoder.so .
cp -f ../build/V192-Correction.json .
echo ""

echo "========================================"
echo "构建成功！"
echo "========================================"
echo ""
echo "可执行文件位置: ./V192Viewer"
echo ""
echo "运行程序:"
echo "  ./V192Viewer"
echo ""
