@echo off
setlocal enabledelayedexpansion

:: 변수 설정
set BUILD_DIR=build
set EXECUTABLE=stans_windows.exe
set DLL_NAME=libedge_client.dll

:: 빌드 디렉토리 생성 및 이동
if not exist %BUILD_DIR% mkdir %BUILD_DIR%
cd %BUILD_DIR%

:: CMake 프로젝트 생성 (MSVC 환경에서 Visual Studio 2022 사용)
cmake -G "Visual Studio 17 2022" ..

:: CMake 빌드 실행 (Release 모드)
cmake --build . --config Release

:: 라이브러리 및 실행 파일 설치
cmake --install . --prefix "..\install"
echo [INFO] Library and executable installed.

:: DLL을 실행 디렉토리(Release)로 복사
if exist "..\install/bin/%DLL_NAME%" (
    echo [INFO] Copying %DLL_NAME% from install/bin to utility/window directory...
    copy "..\install/bin/%DLL_NAME%" "..\utility\window\"
) else (
    echo [ERROR] DLL file not found in install/bin/
)

:: 실행 파일이 정상적으로 생성되었는지 확인
if exist Release\%EXECUTABLE% (
    echo.
    echo ===== Build Success! Running Application =====
    echo.
    cd Release
    %EXECUTABLE%
    cd ..
) else (
    echo.
    echo ===== Build Failed! Check for errors. =====
)

:: 끝나면 대기
pause