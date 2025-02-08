#!/bin/bash

# 스크립트 에러 발생 시 중단
set -e

# 빌드 디렉토리 설정
BUILD_DIR="build"

# 기존 빌드 디렉토리 삭제 (있으면)
if [ -d "$BUILD_DIR" ]; then
    echo "[INFO] Removing existing build directory..."
    rm -rf "$BUILD_DIR"
fi

# 새로운 빌드 디렉토리 생성
echo "[INFO] Creating new build directory..."
mkdir "$BUILD_DIR"
cd "$BUILD_DIR"

# CMake 실행 (Release 모드 빌드)
echo "[INFO] Running CMake..."
cmake -DCMAKE_BUILD_TYPE=Release ..

# nproc이 없는 macOS 지원
if command -v nproc &> /dev/null; then
    CPU_CORES=$(nproc)
else
    CPU_CORES=$(sysctl -n hw.logicalcpu)
fi

# 빌드 실행 (멀티코어 사용)
echo "[INFO] Building project using $CPU_CORES cores..."
make -j"$CPU_CORES"

# 라이브러리 설치
echo "[INFO] Installing libraries..."
make install

echo "[INFO] pwd"
pwd

echo "==== Copying libedge_client.a to ../../utility/linux ===="
cp libedge_client.a ../../utility/linux

# 실행 파일 실행
echo "===== Running Stans ====="
./stans
