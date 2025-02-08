//
// Created by azabell on 2025. 2. 5..
//

#ifndef STANS_EDGE_Client_H
#define STANS_EDGE_Client_H

#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include <memory>
#include <mutex>
#include "scan_result.h"

#ifdef _WIN32
    #ifdef EDGE_CLIENT_EXPORTS
        #define EDGE_CLIENT_API __declspec(dllexport)
    #else
        #define EDGE_CLIENT_API __declspec(dllimport)
    #endif
#else
    #define EDGE_CLIENT_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

class EDGE_CLIENT_API EdgeClient {
public:
    EdgeClient(const std::string &server_ip, unsigned short server_port);
    void setScanResults(const std::vector<ScanResult> &results);
    void scanClientDevices();
    void startScanning();
    void sendToServer();
    void stopScanning();

    std::string getSystemInfo();
    std::string getMemoryInfo();
    std::string getCPUInfo();
    std::string getDiskInfo();

private:
    std::string server_ip;
    unsigned short server_port;
    std::vector<ScanResult> scanResults;

    bool running;
    std::shared_ptr<std::thread> scanningThread;
    std::mutex ClientMutex;

    std::string server_ip_;
    unsigned short server_port_;
};

#ifdef __cplusplus
}
#endif

#endif // STANS_EDGE_Client_H
