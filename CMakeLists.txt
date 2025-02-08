cmake_minimum_required(VERSION 3.20)
project(stans CXX)

# C++20 표준 적용
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 크로스 플랫폼 설정
if (CMAKE_CXX_COMPILER_ID STREQUAL "GNU" OR CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    message(STATUS "Using GCC/Clang Compiler")
    add_compile_options(-std=c++20 -Wall -Wextra -Wpedantic)
    set(PLATFORM_LIBS pthread) # Linux에서 pthread 필요

    # Linux에서 Boost 경로 자동 설정
    set(Boost_ROOT "/usr/local")
    set(Boost_INCLUDE_DIR "/usr/local/include")
    set(Boost_LIBRARY_DIR "/usr/local/lib")
endif()

# Boost 찾기 (경로를 명시적으로 지정)
cmake_policy(SET CMP0167 NEW) # Boost 관련 경고 해결
find_package(Boost REQUIRED COMPONENTS system thread)

if (Boost_FOUND)
    message(STATUS "Boost found at ${Boost_INCLUDE_DIRS}")
    include_directories(${Boost_INCLUDE_DIRS})
    link_directories(${Boost_LIBRARY_DIR})
else()
    message(FATAL_ERROR "Boost not found! Check BOOST_ROOT or install Boost.")
endif()

# edge_client 정적 라이브러리 빌드
if (NOT TARGET edge_client_static)
    add_library(edge_client_static STATIC
        src/edge_client.cpp
        src/lib/edge_client.h
        src/lib/scan_result.h)

    set_target_properties(edge_client_static PROPERTIES OUTPUT_NAME "edge_client")

    install(TARGETS edge_client_static ARCHIVE DESTINATION lib)
endif()

# edge_client 동적 라이브러리 빌드
if (NOT TARGET edge_client_shared)
    add_library(edge_client_shared SHARED
        src/edge_client.cpp
        src/lib/edge_client.h
        src/lib/scan_result.h)

    set_target_properties(edge_client_shared PROPERTIES OUTPUT_NAME "edge_client")

    install(TARGETS edge_client_shared LIBRARY DESTINATION lib)
endif()

# stans 실행 파일 빌드
add_executable(stans
    src/main.cpp)

# stans 실행 파일이 edge_client 및 Boost 라이브러리를 링크하도록 설정
target_link_libraries(stans PRIVATE edge_client_static Boost::system Boost::thread ${PLATFORM_LIBS})

# 설치 경로 설정
install(TARGETS edge_client_static ARCHIVE DESTINATION lib)
install(TARGETS edge_client_shared LIBRARY DESTINATION lib)
install(FILES src/lib/edge_client.h src/lib/scan_result.h DESTINATION include)