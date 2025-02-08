//
// Created by azabell on 2025. 2. 5..
//

#ifndef STANS_SCAN_RESULT_H
#define STANS_SCAN_RESULT_H

#include <string>
#include <vector>

// 로그 항목 정의
struct Entry
{
    std::string serverId;
    int uuid;
};

// 스캔 결과 정의
struct ScanResult
{
    std::string local_ID;
    std::vector<Entry> logList;
};

#endif //STANS_SCAN_RESULT_H
