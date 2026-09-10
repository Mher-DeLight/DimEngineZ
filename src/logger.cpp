#include "../include/logger.h"

namespace DimEngineZ::logger {

std::stringstream outputStream;

void log(std::string_view message) {
    outputStream << message;
}
void logLine(std::string_view message) {
    outputStream << message << "\n";
}

void flushLog(std::string_view message) {
    log(message);
    flush();
}
void flushLogLine(std::string_view message) {
    logLine(message);
    flush();
}

void flush() {
    std::cout << outputStream.rdbuf();
    std::cout << std::flush;
}
void loadToCout() {
    std::cout << outputStream.rdbuf();
}

}; // namespace DimEngineZ::logger