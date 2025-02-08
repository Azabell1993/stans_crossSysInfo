cmake_minimum_required(VERSION 3.20)
project(stans_windows CXX)

# C++20 적용
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# MSVC 환경 설정
if (MSVC)
    message(STATUS "Using MSVC Compiler")
    add_compile_options(/std:c++20 /W4 /MP /utf-8)
    add_definitions(-DWIN32_LEAN_AND_MEAN -D_WIN32_WINNT=0x0601) # Windows 7 이상 지원
endif()

# Boost 라이브러리 찾기
set(Boost_USE_STATIC_LIBS ON)
find_package(Boost REQUIRED COMPONENTS system thread)

if (Boost_FOUND)
    message(STATUS "Boost found at ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIRS})
else()
    message(FATAL_ERROR "Boost not found! Check BOOST_ROOT or install Boost.")
endif()

# nlohmann/json 경로 추가 (경로에 맞게 수정)
include_directories("C:/Users/jeewo/CLionProjects/stans/third_party/nlohmann/include")

# edge_client 동적 라이브러리 빌드 (DLL)
add_library(edge_client_shared SHARED
    ../src/edge_client.cpp
    ../src/lib/edge_client.h
    ../src/lib/scan_result.h)

set_target_properties(edge_client_shared PROPERTIES OUTPUT_NAME "libedge_client")

# Windows에서는 DLL을 위한 전처리기 정의 추가
if (MSVC)
    target_compile_definitions(edge_client_shared PRIVATE EDGE_CLIENT_EXPORTS)
    set_target_properties(edge_client_shared PROPERTIES SUFFIX ".dll")
endif()

install(TARGETS edge_client_shared RUNTIME DESTINATION bin LIBRARY DESTINATION lib)

# Windows 실행 파일 빌드
add_executable(stans_windows ../src/main.cpp)

# 실행 파일이 edge_client_shared 동적 라이브러리를 링크하도록 설정
target_link_libraries(stans_windows PRIVATE edge_client_shared Boost::system Boost::thread)

# 실행 파일과 라이브러리를 같은 경로에 배치
install(TARGETS stans_windows RUNTIME DESTINATION bin)