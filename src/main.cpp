#include <iostream>
#include <memory>
#include <string>
#include <stdexcept>
#if defined(__linux__) || defined(__APPLE__)
    #include "lib/azlog.h"
#endif
#if _WIN32
    #include "lib/win_azlog.h"
    #include <windows.h>
#endif
#include "lib/scan_result.h"
#include "lib/edge_client.h"

int main() {
    try {
        std::string server_ip = "192.168.0.48"; // 우분투 서버 IP
        unsigned short server_port = 12345;     // 서버 포트

        std::vector<ScanResult> scanResults;
        ScanResult result;
        result.local_ID = "Hub_1234";
        result.logList.push_back({"Server_1", 12345});
        scanResults.push_back(result);

        auto ClientService = std::make_shared<EdgeClient>(server_ip, server_port);
        ClientService->setScanResults(scanResults);
#ifdef _WIN32
        std::cout << "[DEBUG] Program running on Windows." << std::endl;
        std::cout <<  "테스트 대상 서버 및 허브 서비스 초기화 중: 서버 IP=" << server_ip << ", 포트=" << server_port << std::endl;
        std::cout << "로컬 서버 및 허브 스캔 시작." << std::endl;
#endif
#if defined(__linux__) || defined(__APPLE__)
        AZLOGDI("테스트 대상 서버 및 허브 서비스 초기화 중: 서버 IP=%s, 포트=%d", "info_log.txt", scanResults, server_ip.c_str(), server_port);
        AZLOGDI("로컬 서버 및 허브 스캔 시작.", "info_log.txt", scanResults);
#endif
        ClientService->startScanning();  // 스캔() 시작

        // 프로그램이 즉시 종료되지 않도록 10초 동안 실행 유지
        std::this_thread::sleep_for(std::chrono::seconds(10));
        ClientService->stopScanning();
        std::cout << "[DEBUG] Program terminating gracefully." << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Caught exception: " << e.what() << std::endl;
    } catch (...) {
        std::cerr << "Caught unknown exception!" << std::endl;
    }

    return 0;
}
