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
