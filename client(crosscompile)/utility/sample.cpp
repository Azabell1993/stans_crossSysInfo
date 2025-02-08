#include <iostream>
#include <string>
#include <fmt/core.h>
#include <spdlog/spdlog.h>
#include "utility/system_info.h"

int main() {
    try {
        std::string cpu_info = getCPUInfo();
        std::string memory_info = getMemoryInfo();
        std::string disk_info = getDiskInfo();
        std::string system_info = getSystemInfo();

        spdlog::info("CPU Info: {}", cpu_info);
        spdlog::info("Memory Info: {}", memory_info);
        spdlog::info("Disk Info: {}", disk_info);
        spdlog::info("System Info: {}", system_info);
    } catch (const std::exception& e) {
        spdlog::error("Exception caught: {}", e.what());
    }
    return 0;
}