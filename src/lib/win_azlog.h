#ifndef STANS_AZLOG_H
#define STANS_AZLOG_H

#include <stdio.h>
#include <wchar.h>
#include <iostream>
#include <string>
#include <memory>
#include <mutex>
#include <ctime>
#include <fstream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <cstring>
#include <format>

#ifdef _WIN32
    #include <windows.h>
    #define mkdir(path, mode) _mkdir(path)
#else
    #include <unistd.h>
    #include <sys/stat.h>
    #include <syslog.h>
    #include <errno.h>
#endif
#include "scan_result.h"

#define AZLOGD_BUF_SIZE 4096

struct CURRENT_T {
    int year, month, day, hour, minute, second;
};

static inline void getCurrentTime(CURRENT_T *current) {
    time_t now = time(nullptr);
    struct tm local_tm;
    #ifdef _WIN32
        localtime_s(&local_tm, &now);
    #else
        localtime_r(&now, &local_tm);
    #endif
    current->year = local_tm.tm_year + 1900;
    current->month = local_tm.tm_mon + 1;
    current->day = local_tm.tm_mday;
    current->hour = local_tm.tm_hour;
    current->minute = local_tm.tm_min;
    current->second = local_tm.tm_sec;
}

static inline void ensureDirectoryExists(const std::string &dirPath) {
#ifdef _WIN32
    CreateDirectory(dirPath.c_str(), NULL);
#else
    struct stat info;
    if (stat(dirPath.c_str(), &info) == 0 && (info.st_mode & S_IFDIR)) return;
    if (mkdir(dirPath.c_str(), 0777) == -1)
        std::cerr << "Failed to create directory: " << dirPath << "\n";
#endif
}

#ifdef _WIN32
    static inline std::string formatString(const char *format_str, std::vector<std::string> args) {
        std::ostringstream ss;
        const char *p = format_str;
        size_t index = 0;
        while (*p) {
            if (*p == '%' && *(p + 1) == 's' && index < args.size()) {
                ss << args[index++];
                p += 2;
            } else {
                ss << *p++;
            }
        }
        return ss.str();
    }
#endif

static inline void COUT_(std::string log_level, std::string function, int line, const char *format_str, const std::string &locationLogUrl, std::vector<ScanResult> scanResults, ...) {
    std::string logMessage;
    
#ifdef _WIN32
    std::vector<std::string> args = {locationLogUrl};
    logMessage = formatString(format_str, args);
#else
    va_list ap;
    char buf[AZLOGD_BUF_SIZE];
    va_start(ap, scanResults);
    vsnprintf(buf, AZLOGD_BUF_SIZE, format_str, ap);
    va_end(ap);
    logMessage = buf;
#endif

    CURRENT_T current;
    getCurrentTime(&current);
    static std::mutex coutWriteMutex;
    std::string logDir = "./logs";
    std::string filePath = logDir + "/" + locationLogUrl;
    ensureDirectoryExists(logDir);

    {
        std::lock_guard<std::mutex> lock(coutWriteMutex);
        std::ostringstream rowBuilder;
        rowBuilder << "[" << current.year << "/" << current.month << "/" << current.day << " "
                   << current.hour << ":" << current.minute << ":" << current.second << "] "
                   << log_level << " (" << function << ":" << line << ") - " << logMessage;
        
        if (!scanResults.empty()) {
            rowBuilder << " [ScanResults: ";
            for (const auto &result : scanResults) {
                rowBuilder << "HubId=" << result.local_ID << ". Logs=";
                for (const auto &entry : result.logList) {
                    rowBuilder << std::format("{:+06d} ", entry.uuid);
                }
                rowBuilder << "; ";
            }
            rowBuilder << "]";
        }
        rowBuilder << "\n";

        std::cout << rowBuilder.str();
        std::ofstream outFile(filePath, std::ios::app);
        if (outFile.is_open()) {
            outFile << rowBuilder.str();
            outFile.close();
        } else {
            std::cerr << "Failed to open log file: " << filePath << "\n";
        }
    }
}

#endif //STANS_AZLOG_H