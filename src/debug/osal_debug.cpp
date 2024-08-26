//
// Created by kamin.deng on 2024/8/23.
//
#include "osal_debug.h"

namespace osal {

// 当前日志等级
static LogLevel currentLogLevel = LOG_LEVEL_INFO;

// 是否输出文件名、函数名和行数
static bool includeFileFunctionLine = false;

// 设置日志等级
void setLogLevel(LogLevel level) { currentLogLevel = level; }

LogLevel getLogLevel() { return currentLogLevel; }

// 设置是否输出文件名、函数名和行数
void setIncludeFileFunctionLine(bool include) { includeFileFunctionLine = include; }

bool getIncludeFileFunction() { return includeFileFunctionLine; }

// 通用日志函数
void common_log(const char *prefix, const char *file, const char *function, int line, const char *format,
                va_list args) {
    char buf[512];    // 定义一个足够大的缓冲区来存储整个日志消息
    char *ptr = buf;  // 使用指针来跟踪buf中的当前位置

    // 构建前缀
    if (getIncludeFileFunction()) {
        ptr += snprintf(ptr, sizeof(buf) - (ptr - buf), "[%s:%s:%d] ", file, function, line);
    }

    // 复制prefix到buf中（如果prefix非空且还有空间）
    if (prefix != NULL && *prefix != '\0' && (sizeof(buf) - (ptr - buf)) > 0) {
        ptr += snprintf(ptr, sizeof(buf) - (ptr - buf), "%s", prefix);
    }

    int written = 0;
    // 如果prefix和前缀信息之后还有空间，则格式化日志消息
    if ((sizeof(buf) - (ptr - buf)) > 0) {
        va_list args_copy;
        va_copy(args_copy, args);
        written = vsnprintf(ptr, sizeof(buf) - (ptr - buf), format, args_copy);
        va_end(args_copy);
    }

    // 计算实际需要发送的字节数
    int total_written = ptr - buf + written;

    // 输出日志
    osal_port_debug_write(buf, total_written);
}

// 注意：
// 1. 确保buf的大小足够大，以容纳所有可能的内容。
// 2. 在实际使用中，你可能需要添加额外的错误检查来处理可能的缓冲区溢出。
// 3. 如果日志消息可能非常大，这种方法可能不适用，因为你需要一个固定大小的缓冲区。
//    在这种情况下，你可能需要考虑使用动态内存分配或其他日志记录机制。
// 定义具体的日志函数
DEFINE_LOG_FUNCTION(OSAL_LOG_, "", LOG_LEVEL_INFO)

DEFINE_LOG_FUNCTION(OSAL_LOGA_, "LOGA: ", LOG_LEVEL_VERBOSE)

DEFINE_LOG_FUNCTION(OSAL_LOGE_, "LOGE: ", LOG_LEVEL_ERROR)

DEFINE_LOG_FUNCTION(OSAL_LOGW_, "LOGW: ", LOG_LEVEL_WARNING)

DEFINE_LOG_FUNCTION(OSAL_LOGI_, "LOGI: ", LOG_LEVEL_INFO)

DEFINE_LOG_FUNCTION(OSAL_LOGD_, "LOGD: ", LOG_LEVEL_DEBUG)

DEFINE_LOG_FUNCTION(OSAL_LOGV_, "LOGV: ", LOG_LEVEL_VERBOSE)

}  // namespace osal