//
// Created by azabell on 2025. 2. 5..
//
#if defined(__linux__) || defined(__APPLE__)
    #include "lib/azlog.h"
#endif
#include "lib/scan_result.h"
#include "lib/edge_client.h"
#include <boost/asio.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <cstring>
#include <mutex>

#include <nlohmann/json.hpp>

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


using json = nlohmann::json;
using boost::asio::ip::tcp;

EdgeClient::EdgeClient(const std::string &server_ip, unsigned short server_port)
        : running(false), server_ip_(server_ip), server_port_(server_port)
{
    scanResults.clear();

#ifdef __WIN32
    std::cout << "[DEBUG] Running on Windows" << std::endl;
#endif

#ifdef __linux__
    std::cout << "[DEBUG] Running on Linux" << std::endl;
    AZLOGDI("EdgeClient initialized with empty scanResults.", "debug_log.txt", {});
#endif

#ifdef __APPLE__
    std::cout << "[DEBUG] Running on macOS" << std::endl;
    AZLOGDI("EdgeClient initialized with empty scanResults.", "debug_log.txt", {});
#endif

}

/**
 * @brief BLE 장치 스캔을 시작하는 함수
 * @details BLE 장치 스캔을 시작하고 스레드를 생성
*/
void EdgeClient::startScanning()
{
    running = true;

    // 이미 실행 중이면 다시 시작하지 않음
    if (!scanningThread || !scanningThread->joinable())
    {
        scanningThread = std::make_shared<std::thread>(&EdgeClient::scanClientDevices, this);
        std::cout << "[DEBUG] Scanning thread started." << std::endl;
    }
}

/**
 * @brief BLE 장치 스캔을 중지하는 함수
 * @details BLE 장치 스캔을 중지하고 스레드를 종료
*/
void EdgeClient::stopScanning()
{
    running = false;
    if (scanningThread && scanningThread->joinable())
    {
        std::cout << "[DEBUG] Stopping scanning thread..." << std::endl;
        scanningThread->join();  // 스레드가 완전히 종료될 때까지 대기
        std::cout << "[DEBUG] Scanning thread stopped." << std::endl;
    }
}

/**
 * @brief BLE 장치를 스캔하는 함수
 * @details BLE 장치를 스캔하여 서버로 전송
*/
void EdgeClient::scanClientDevices()
{
    std::cout << "[DEBUG] scanClientDevices() started..." << std::endl;

    while (running)
    {
        std::cout << "[DEBUG] Inside while loop of scanClientDevices()" << std::endl;
        sendToServer();
        std::this_thread::sleep_for(std::chrono::seconds(2));  // CPU 과부하 방지
    }

    std::cout << "[DEBUG] scanClientDevices() exited the loop." << std::endl;
}

/**
 * @brief 테스트용 Hello, World! 문자열을 설정하는 함수
 * @param str Hello, World! 문자열
 * @return Hello, World! 문자열
*/
// std::string EdgeClient::setTestHelloWorld(const std::string &str)
// {
//     auto helloLambda = []() -> std::string {
//         return "Hello, World!";
//     };

//     std::cout << helloLambda() << std::endl;
//     return helloLambda();
// }

/**
 * @brief 스캔 결과를 설정하는 함수
 * @param results 스캔 결과
*/
void EdgeClient::setScanResults(const std::vector<ScanResult> &results)
{
    std::lock_guard<std::mutex> lock(ClientMutex);
    scanResults = results;

#ifdef _WIN32
    printf("[DEBUG] Scan results set: Size=%d\n", static_cast<int>(scanResults.size()));
    for (const auto &result : scanResults)
    {
        printf("[DEBUG] HubId: %s, Logs Count: %d\n", result.local_ID.c_str(), static_cast<int>(result.logList.size()));
    }
#else
    // 전체 scanResults 전달
    AZLOGDI("Scan results set", "debug_log.txt", scanResults);

    // 단일 ScanResult 전달
    for (const auto &result : scanResults) {
        AZLOGDI("HubId: %s, Logs Count: %d", "debug_log.txt", std::vector<ScanResult>{result}, result.local_ID.c_str(), static_cast<int>(result.logList.size()));
    }
#endif
}

/**
 * @brief 테스트용 Hello, World! 문자열을 반환하는 함수
 * @return Hello, World! 문자열
*/
// std::string EdgeClient::getTestHelloWorld()
// {
//     bool testFlag = true;
//     std::string str;

//     try {
//         if(testFlag) {
//             str = setTestHelloWorld(str);  // 반환값을 변수에 저장
//             std::cout << "getTestHelloWorld" << std::endl;
//         }
//     } catch (const std::exception& e) {
//         std::cerr << "Caught exception: " << e.what() << std::endl;
//     } catch (...) {
//         std::cerr << "Caught unknown exception!" << std::endl;
//     }

//     return str;
// }

/**
 * @brief CPU 정보를 반환하는 함수
 * @return CPU 정보를 JSON 형태로 반환
 * @details CPU 모델명, 코어 수, 사용량을 반환
*/
std::string EdgeClient::getCPUInfo() {
    std::ostringstream cpuInfo;
    std::string modelName;
    int cpuCores = 0;
    double cpuUsage = 0.0;

#ifdef _WIN32
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    cpuCores = sysInfo.dwNumberOfProcessors;
    modelName = "Windows CPU";

    FILETIME idleTime, kernelTime, userTime;
    if (GetSystemTimes(&idleTime, &kernelTime, &userTime)) {
        ULARGE_INTEGER idle, kernel, user;
        idle.LowPart = idleTime.dwLowDateTime; idle.HighPart = idleTime.dwHighDateTime;
        kernel.LowPart = kernelTime.dwLowDateTime; kernel.HighPart = kernelTime.dwHighDateTime;
        user.LowPart = userTime.dwLowDateTime; user.HighPart = userTime.dwHighDateTime;

        static ULARGE_INTEGER prevIdle = idle, prevKernel = kernel, prevUser = user;
        cpuUsage = 100.0 * (1.0 - (double)(idle.QuadPart - prevIdle.QuadPart) /
                            (kernel.QuadPart + user.QuadPart - prevKernel.QuadPart - prevUser.QuadPart));
        prevIdle = idle; prevKernel = kernel; prevUser = user;
    }
#endif
#ifdef __linux__
    std::ifstream cpuinfo("/proc/cpuinfo");
    std::string line;
    while (std::getline(cpuinfo, line)) {
        if (line.find("model name") != std::string::npos) {
            size_t pos = line.find(":");
            if (pos != std::string::npos) {
                modelName = line.substr(pos + 2);
            }
        }
        if (line.find("processor") != std::string::npos) {
            cpuCores++;
        }
    }

    std::ifstream statfile("/proc/stat");
    unsigned long long user, nice, system, idle, iowait, irq, softirq, steal;
    statfile >> line >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal;
    unsigned long long totalIdle = idle + iowait;
    unsigned long long totalCpu = user + nice + system + idle + iowait + irq + softirq + steal;

    std::this_thread::sleep_for(std::chrono::milliseconds(500));

    std::ifstream statfile2("/proc/stat");
    unsigned long long user2, nice2, system2, idle2, iowait2, irq2, softirq2, steal2;
    statfile2 >> line >> user2 >> nice2 >> system2 >> idle2 >> iowait2 >> irq2 >> softirq2 >> steal2;
    unsigned long long totalIdle2 = idle2 + iowait2;
    unsigned long long totalCpu2 = user2 + nice2 + system2 + idle2 + iowait2 + irq2 + softirq2 + steal2;

    cpuUsage = (1.0 - (double)(totalIdle2 - totalIdle) / (totalCpu2 - totalCpu)) * 100.0;
#endif
#ifdef __APPLE__
    char buffer[128];
    size_t bufferSize = sizeof(buffer);
    sysctlbyname("machdep.cpu.brand_string", &buffer, &bufferSize, NULL, 0);
    modelName = buffer;
    int coreCount;
    size_t coreSize = sizeof(coreCount);
    sysctlbyname("hw.logicalcpu", &coreCount, &coreSize, NULL, 0);
    cpuCores = coreCount;

    // macOS CPU 사용량 계산
    host_cpu_load_info_data_t cpuLoad;
    mach_msg_type_number_t count = HOST_CPU_LOAD_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_CPU_LOAD_INFO, (host_info_t)&cpuLoad, &count) == KERN_SUCCESS)
    {
        unsigned long long totalTicks = cpuLoad.cpu_ticks[CPU_STATE_USER] +
                                        cpuLoad.cpu_ticks[CPU_STATE_SYSTEM] +
                                        cpuLoad.cpu_ticks[CPU_STATE_IDLE];

        unsigned long long usedTicks = totalTicks - cpuLoad.cpu_ticks[CPU_STATE_IDLE];
        cpuUsage = (double)usedTicks / totalTicks * 100.0;
    }
#endif
    cpuInfo << "\"Model\": \"" << modelName << "\", ";
    cpuInfo << "\"Cores\": " << cpuCores << ", ";
    cpuInfo << "\"Usage\": " << cpuUsage;
    return cpuInfo.str();
}

std::string EdgeClient::getMemoryInfo() {
    std::ostringstream memoryInfo;
    long long totalMemory = 0, usedMemory = 0;

#ifdef _WIN32
    MEMORYSTATUSEX memInfo;
    memInfo.dwLength = sizeof(memInfo);
    GlobalMemoryStatusEx(&memInfo);
    totalMemory = memInfo.ullTotalPhys;
    usedMemory = totalMemory - memInfo.ullAvailPhys;
#endif

#ifdef __linux__
    struct sysinfo memInfo;
    sysinfo(&memInfo);
    totalMemory = memInfo.totalram * memInfo.mem_unit;
    usedMemory = (memInfo.totalram - memInfo.freeram) * memInfo.mem_unit;
#elif __APPLE__
    int64_t memSize;
    size_t memSizeLen = sizeof(memSize);
    sysctlbyname("hw.memsize", &memSize, &memSizeLen, NULL, 0);
    totalMemory = memSize;

    vm_statistics64_data_t vmStat;
    mach_msg_type_number_t count = HOST_VM_INFO_COUNT;
    if (host_statistics(mach_host_self(), HOST_VM_INFO, (host_info_t)&vmStat, &count) == KERN_SUCCESS)
    {
        usedMemory = totalMemory - (vmStat.free_count * sysconf(_SC_PAGESIZE));
    }
#endif

    memoryInfo << "\"Total\": " << totalMemory << ", ";
    memoryInfo << "\"Used\": " << usedMemory;
    return memoryInfo.str();
}

std::string EdgeClient::getDiskInfo() {
    std::ostringstream diskInfo;
    std::vector<std::string> disks;
#ifdef _WIN32
    disks = {"C:\\", "D:\\"};
#elif __linux__
    disks = {"/", "/home", "/var", "/tmp"};
#elif __APPLE__
    disks = {"/", "/System/Volumes/Data"};
#endif
    diskInfo << "{";
    for (size_t i = 0; i < disks.size(); i++) {
        long long totalDisk = 0, availableDisk = 0;
#ifdef _WIN32
        ULARGE_INTEGER freeBytes, totalBytes, totalFreeBytes;
        if (GetDiskFreeSpaceEx(disks[i].c_str(), &freeBytes, &totalBytes, &totalFreeBytes)) {
            totalDisk = totalBytes.QuadPart;
            availableDisk = freeBytes.QuadPart;
        }
#elif defined(__linux__) || defined(__APPLE__)
        struct statvfs diskStat;
        if (statvfs(disks[i].c_str(), &diskStat) == 0) {
            totalDisk = diskStat.f_blocks * diskStat.f_frsize;
            availableDisk = diskStat.f_bavail * diskStat.f_frsize;
        }
#endif
        diskInfo << "\"" << disks[i] << "\": {";
        diskInfo << "\"Total\": " << totalDisk << ", ";
        diskInfo << "\"Available\": " << availableDisk << "}";
        if (i != disks.size() - 1) diskInfo << ", ";
    }
    diskInfo << "}";
    return diskInfo.str();
}

std::string EdgeClient::getSystemInfo() {
    std::ostringstream systemInfo;
    systemInfo << "{";
    systemInfo << "\"CPU\": {" << getCPUInfo() << "}, ";
    systemInfo << "\"Memory\": {" << getMemoryInfo() << "}, ";
    systemInfo << "\"Disk\": " << getDiskInfo();
    systemInfo << "}";
    return systemInfo.str();
}

/**
 * @brief 서버로 데이터를 전송하는 함수
 * @details 서버로 데이터를 전송하는 함수
*/
void EdgeClient::sendToServer()
{
    std::lock_guard<std::mutex> lock(ClientMutex);
    std::cout << "[DEBUG] Inside sendToServer()" << std::endl;

//    std::string message = getTestHelloWorld();  // "Hello, World!" 반환
    std::string message = getSystemInfo(); //  본 과제 정보

    if (message.empty())
    {
        std::cerr << "Error: Empty message, nothing to send!" << std::endl;
#if defined(__linux__) || defined(__APPLE__)
        AZLOGDE("Error: Empty message, nothing to send!", "error_log.txt", scanResults);
#endif
        return;
    }

    try
    {
        boost::asio::io_context io_context;

        // 서버 주소 및 포트 설정
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve(server_ip_, std::to_string(server_port_));
        auto socket = std::make_shared<tcp::socket>(io_context);
        boost::asio::connect(*socket, endpoints);

        uint32_t data_size = static_cast<uint32_t>(message.size());
        uint32_t data_size_network_order = htonl(data_size);

        std::cout << "[Client] Sending data size: " << data_size << " bytes" << std::endl;

        // 데이터 크기 전송
        boost::asio::write(*socket, boost::asio::buffer(&data_size_network_order, sizeof(data_size_network_order)));

        // 메시지 전송
        boost::asio::write(*socket, boost::asio::buffer(message));

        std::cout << "[Client] Message sent successfully: " << message << std::endl;
    }
    catch (const std::exception &e)
    {
        std::cerr << "[Client] Error in sendToServer: " << e.what() << std::endl;
    }
}

