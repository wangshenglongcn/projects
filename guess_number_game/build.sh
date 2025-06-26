#!/bin/bash

set -e
set -u

# 输出当前目录
echo "Current directory is: $(pwd)"

# 删除旧的 build 目录
if [[ -d build ]]; then
    echo "Removing existing build directory..."
    rm -rf build
fi

# 创建build目录
mkdir build
cd build

# 运行 cmake 并检查是否成功
echo "Configuring project with CMake..."
if ! cmake .. -G "MinGW Makefiles"; then
    echo "CMake configuration failed!"
    exit 1
fi

# 编译项目
echo "Building with mingw32-make..."
if ! mingw32-make; then
    echo "Build failed!"
    exit 1
fi

echo "Build succeeded!"