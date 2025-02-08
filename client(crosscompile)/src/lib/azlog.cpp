#if !defined(_WIN32)  // 리눅스 및 macOS에서만 컴파일
    #include "azlog.h"

    // 로그 출력 함수
    static inline void COUT_(const std::string& log_level, const std::string& function, int line, const char *format_str, ...) {
        char buf[AZLOGD_BUF_SIZE];

        // 가변 인수 처리
        va_list args;
        va_start(args, format_str);
        vsnprintf(buf, AZLOGD_BUF_SIZE, format_str, args);
        va_end(args);

        CURRENT_T current;
        getCurrentTime(&current);

        std::ostringstream logMsg;
        logMsg << "[" << current.year << "/" << current.month << "/" << current.day << " "
            << current.hour << ":" << current.minute << ":" << current.second << "] "
            << log_level << " (" << function << ":" << line << ") - " << buf << std::endl;

        std::cout << logMsg.str();
    }

    // 로그 매크로 정의
    #define AZLOGD(format_str, ...) COUT_("DEBUG", __func__, __LINE__, format_str, ##__VA_ARGS__)
#endif