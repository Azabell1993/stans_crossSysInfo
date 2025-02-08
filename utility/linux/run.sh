#!/bin/bash

# 스크립트 에러 발생 시 중단
set -e

# 빌드 실행
g++ -std=c++20 -Wall -Wextra -Wpedantic \
    -o sample Sample.cpp -L. -ledge_client -lfmt -lspdlog -pthread
