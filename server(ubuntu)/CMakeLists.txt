cmake_minimum_required(VERSION 3.14)
project(server)

# C++20 설정
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# OpenCV 찾기
find_package(OpenCV REQUIRED)

# Boost 찾기 (system 모듈 필수)
find_package(Boost REQUIRED COMPONENTS system)

# 실행 파일 추가
add_executable(server server.cpp)

# MSVC 및 GCC 호환을 위한 라이브러리 연결
target_link_libraries(server
    PRIVATE
    ${OpenCV_LIBS}
    Boost::system
)

# Boost 헤더 경로 추가 (GCC에서는 필요할 수 있음)
if (Boost_FOUND)
    target_include_directories(server PRIVATE ${Boost_INCLUDE_DIRS})
endif()