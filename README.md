
# 시스템 정보 수집기 (System Information Collector)

유튜브 링크 : https://youtu.be/WDvIzgrPxZA

## 방법 1. 클라이언트 <-> 서버 통신

### 서버에서 받은 result.txt 모습
https://github.com/Azabell1993/stans_crossSysInfo/blob/0e226863757acb5502f61cf50541b1895e5ca638/result.txt#L40-L49

## 개요

이 프로젝트는 크로스 플랫폼 시스템 정보 수집기입니다. 클라이언트에서 시스템 정보를 수집하여 서버로 전송하며, Windows(MSVC), Linux(GCC), macOS에서 동작하도록 설계되었습니다. 
> C++20 표준을 준수합니다.(client측 CMake 참조)

---

## 코드 개요

### 클라이언트
- **Client 스캔**
  - `scanningThread`를 `std::make_shared<std::thread>`로 생성.
  - `scanClientDevices()`에서 `sendToServer()`을 주기적으로 호출.

- **데이터 전송**
  - `boost::asio`를 사용하여 서버와 연결.
  - "Hello, World!" 메시지를 생성하고 전송.(테스트 용, 주석 처리)
  - sysInfo 정보를 각 아키텍처별 구분하여 서버로 메시지 생성 후 전송. 

### 서버
- 클라이언트로부터 데이터 수신 및 처리.
- 데이터를 `result.txt`로 저장.

아래는 요청하신 내용을 포함한 `README.md` 파일입니다. 실제 저장된 `result.txt`와 요구된 시스템 정보가 일치하는지를 확인하고 이에 대해 기술하였습니다.

---

## 주요 기능

### 클라이언트
1. **시스템 정보 수집**
   - **CPU**: 모델명, 코어 수, 사용률.
   - **메모리**: 총 용량, 현재 사용량.
   - **디스크**: 디스크 목록, 디스크별 총 용량 및 사용 가능 공간.

2. **Client 기반 데이터 전송**
   - Client 장치 스캔(블루투스로 지칭).
   - 서버로 시스템 정보 전송.

3. **메시지 전송**
   - "Hello, World!" 메시지 생성 및 전송.
   - 데이터 크기와 메시지를 순차적으로 전송.

4. **크로스 컴파일 지원**
   - macOS 는 Clang을 사용.
   - Linux는 GCC를 사용.
   - Windows는 MSVC를 사용.

### 서버
1. **데이터 처리**
   - 클라이언트로부터 전송된 시스템 정보 수신 및 처리.
   - `result.txt` 파일로 데이터 저장.

2. **Client 장치 스캔**
   - 전용 스레드를 통해 Client 장치 스캔.

3. **CLI 기반 빌드**
   - `run.sh`를 사용한 수동 빌드 지원.

---

## 빌드 및 실행 방법

### 의존성
- **C++20 컴파일러**: GCC(Linux), CLang(macOS), MSVC(Windows).
- **CMake**: 빌드 구성용.
- **Boost.Asio**: 네트워크 통신용.

### 클라이언트
1. **자동 빌드**:
   - CLion을 사용하여 간단히 빌드 가능.
2. **수동 빌드**:
   - 제공된 `run.sh` 스크립트를 실행.

### 서버
1. **수동 빌드**:
   - Ubuntu 또는 macOS에서 `run.sh`를 실행하여 컴파일 및 실행.

---

## 사용 방법

### 클라이언트
1. 클라이언트 소스 파일을 열어 아래 IP 주소를 설정:
   ```cpp
   std::string server_ip = "192.168.0.48"; // 서버 IP로 수정
   ```
2. 클라이언트를 컴파일 및 실행. 서버가 실행 중이지 않으면 다음과 같은 오류 메시지가 발생할 수 있음:
   ```
   [Client] Error in sendToServer: connect: Connection refused [system:61 at /usr/local/include/boost/asio/detail/reactive_socket_service.hpp:587:33 in function 'boost::system::error_code boost::asio::detail::reactive_socket_service<boost::asio::ip::tcp>::connect(implementation_type &, const endpoint_type &, boost::system::error_code &) [Protocol = boost::asio::ip::tcp]']
   ```

### 서버
1. `run.sh`를 실행하여 서버 빌드 및 실행.
2. 서버는 클라이언트로부터 수신된 데이터를 `result.txt`에 저장.

---

### 수집되는 시스템 정보(macOS 기준)
1. **CPU 정보**
   - **모델명**: `Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz` (예시)
   - **코어 수**: `12`
   - **사용량(%)**: `6.61789` (예시)

2. **메모리 정보**
   - **총 용량**: `34359738368` (34GB)
   - **현재 사용량**: `34359738368` (34GB)

3. **디스크 정보**
   - **디스크 목록 및 각 디스크의 용량**
     - `/`:
       - 총 용량: `499963174912` (500GB)
       - 사용 가능 용량: `245298565120` (245GB)
     - `/System/Volumes/Data`:
       - 총 용량: `499963174912` (500GB)
       - 사용 가능 용량: `245298565120` (245GB)

---

### 데이터 수집 및 저장 예시

#### 서버 측 `result.txt` 내용(/home/azabell/Desktop/stans/server/build/result.txt)
```json
{"CPU": {"Model": "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz", "Cores": 12, "Usage": 6.61789}, "Memory": {"Total": 34359738368, "Used": 34359738368}, "Disk": {"/": {"Total": 499963174912, "AvailaClient": 245298565120}, "/System/Volumes/Data": {"Total": 499963174912, "AvailaClient": 245298565120}}}
{"CPU": {"Model": "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz", "Cores": 12, "Usage": 6.67121}, "Memory": {"Total": 34359738368, "Used": 34359738368}, "Disk": {"/": {"Total": 499963174912, "AvailaClient": 244263350272}, "/System/Volumes/Data": {"Total": 499963174912, "AvailaClient": 244263350272}}}
{"CPU": {"Model": "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz", "Cores": 12, "Usage": 6.67155}, "Memory": {"Total": 34359738368, "Used": 34359738368}, "Disk": {"/": {"Total": 499963174912, "AvailaClient": 244263301120}, "/System/Volumes/Data": {"Total": 499963174912, "AvailaClient": 244263301120}}}
```

#### 분석
- **CPU 정보**:
  - `Model`: CPU 모델명이 정확히 포함되어 있습니다.
  - `Cores`: 코어 수 정보가 정확히 포함되어 있습니다.
  - `Usage`: CPU 사용량은 소수점 이하 5자리로 기록되어 있습니다.

- **메모리 정보**:
  - 총 용량(`Total`) 및 사용량(`Used`) 정보가 정확히 포함되어 있습니다.
  - 데이터 값이 실제 메모리 크기와 일치합니다.

- **디스크 정보**:
  - 디스크 경로(`/`, `/System/Volumes/Data`)가 정확히 기록되었습니다.
  - 각 디스크의 총 용량(`Total`) 및 사용 가능 용량(`AvailaClient`)이 명시되어 있습니다.

---

## REQUEST, RESPONSE

### 서버 측 로그(RESPONSE)
```bash
azabell@azabell-ubuntu:~/Desktop/stans/server/build$ ./server
Running this Server Program...
need the clientPort : 12345
Server connected: 192.168.0.22:50680
[Client] Expected data size: 291 bytes
[Client] Read 291 bytes, total: 291/291 bytes
[Client] Received message saved to result.txt: {"CPU": {"Model": "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz", "Cores": 12, "Usage": 6.61789}, "Memory": {"Total": 34359738368, "Used": 34359738368}, "Disk": {"/": {"Total": 499963174912, "AvailaClient": 245298565120}, "/System/Volumes/Data": {"Total": 499963174912, "AvailaClient": 245298565120}}}
Response sent.
```

### 클라이언트 측 로그(REQUEST)
```text
[DEBUG] Running on macOS
[Client] Sending data size: 291 bytes
[Client] Message sent successfully: {"CPU": {"Model": "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz", "Cores": 12, "Usage": 6.67121}, "Memory": {"Total": 34359738368, "Used": 34359738368}, "Disk": {"/": {"Total": 499963174912, "AvailaClient": 244263350272}, "/System/Volumes/Data": {"Total": 499963174912, "AvailaClient": 244263350272}}}
```

---

## 윈도우도 가능합니다.
> https://github.com/Azabell1993/stans_crossSysInfo/tree/main/client(request_crosscompile)/window
에서 .\run.bat을 터미널 혹은 VSCODE에서 실행하시면 가능합니다.
main.cpp 및 코드는 그대로 유지가 가능하며, 아래처럼 세부 전처리기를 통하여 아키텍처별 컴파일을 유도하였습니다.
(MAC, Linux, Window)

```
#ifdef _WIN32
        std::cout << "[DEBUG] Program running on Windows." << std::endl;
        std::cout <<  "테스트 대상 서버 및 허브 서비스 초기화 중: 서버 IP=" << server_ip << ", 포트=" << server_port << std::endl;
        std::cout << "로컬 서버 및 허브 스캔 시작." << std::endl;
#endif
#if defined(__linux__) || defined(__APPLE__)
        AZLOGDI("테스트 대상 서버 및 허브 서비스 초기화 중: 서버 IP=%s, 포트=%d", "info_log.txt", scanResults, server_ip.c_str(), server_port);
        AZLOGDI("로컬 서버 및 허브 스캔 시작.", "info_log.txt", scanResults);
#endif
```

1) AZLOGDI는 `va_list` 가변인자 혹은 리눅스에서만 사용이 가능한 헤더가 있으므로 리눅스에서만 작동
2) WINDOW에서는 `AZLOGDI` 따위의 커스텀한 로그를 사용할 수 없음.

### 아키텍처별 구분
```
#ifdef _WIN32
#include <windows.h>
#include <sysinfoapi.h>
#endif

#ifdef __linux__
#include <sys/sysinfo.h>
#include <sys/statvfs.h>
#include <unistd.h>
#endif

#ifdef __APPLE__
#include <sys/sysctl.h>
#include <sys/mount.h>
#include <mach/mach.h>
#include <sys/statvfs.h>
#endif
```

또한, third_party라는 폴더 명시를 확인할 수가 있는데 이에 대해서는 외부 라이브러리를 가져다 쓴 것 입니다.

1) 맥
```
brew install nlohmann-json
brew list | grep nlohmann-json
```
2) 리눅스
```
sudo apt update
sudo apt install nlohmann-json3-dev
```
3) 윈도우 
   - third_party 폴더 생성 -> git clone https://github.com/nlohmann/json.git
   - json폴더 이름을 nlohmann으로 변경

### 결과(서버) - D드라이브가 없으므로 0으로 표기됨.
```
Server connected: 192.168.0.31:53031
[Client] Expected data size: 225 bytes
[Client] Read 225 bytes, total: 225/225 bytes
[Client] Received message saved to result.txt: {"CPU": {"Model": "Windows CPU", "Cores": 12, "Usage": 3.35917}, "Memory": {"Total": 16539570176, "Used": 11084660736}, "Disk": {"C:\": {"Total": 511176601600, "AvailaClient": 373145038848}, "D:\": {"Total": 0, "AvailaClient": 0}}}
Response sent.
```

------

## 방법 2. 방법(1)에서 크로스 컴파일을 통해 빌드한 `libedge_client.a`를 활용하여 spdlog랑 fmt 라이브러리를 head-only로 적용한 (리눅스 버전의) Sample.cpp
```
#include <iostream>
#include <memory>
#include <fmt/core.h>
#include <spdlog/spdlog.h>

// EdgeClient 클래스 선언 (헤더 없이 사용)
class EdgeClient {
public:
    EdgeClient(const std::string& server_ip, unsigned short server_port);
    std::string getCPUInfo();
    std::string getMemoryInfo();
    std::string getDiskInfo();
};

// 메인 실행 함수
int main() {
    spdlog::info("Starting Sample Program...");

    // 안전한 메모리 관리를 위한 unique_ptr 사용
    auto client = std::make_unique<EdgeClient>("127.0.0.1", 12345);

    // 시스템 정보 조회
    fmt::print("=== System Information ===\n");
    fmt::print("CPU Info: {}\n", client->getCPUInfo());
    fmt::print("Memory Info: {}\n", client->getMemoryInfo());
    fmt::print("Disk Info: {}\n", client->getDiskInfo());

    spdlog::info("Sample Program Completed.");
    return 0;
}
```

### 위의 기능을 사용하기 위한 `run.sh`
```
#!/bin/bash

# 스크립트 에러 발생 시 중단
set -e

# 빌드 실행
g++ -std=c++20 -Wall -Wextra -Wpedantic \
    -o sample Sample.cpp -L. -ledge_client -lfmt -lspdlog -pthread
```

### 결과물
```
azabell@azabellui-MacBookPro linux % ./run.sh 
azabell@azabellui-MacBookPro linux % ./sample
[2025-02-08 23:33:48.707] [info] Starting Sample Program...
[DEBUG] Running on macOS
[2025/2/8 23:33:48] INFO (EdgeClient:57) - EdgeClient initialized with empty scanResults.
=== System Information ===
CPU Info: "Model": "Intel(R) Core(TM) i7-9750H CPU @ 2.60GHz", "Cores": 12, "Usage": 3.72006
Memory Info: "Total": 34359738368, "Used": 30238330880
Disk Info: {"/": {"Total": 499963174912, "Available": 246283329536}, "/System/Volumes/Data": {"Total": 499963174912, "Available": 246283329536}}
[2025-02-08 23:33:48.708] [info] Sample Program Completed.
```