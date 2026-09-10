#include "../include/logger.h"

namespace DimEngineZ::logger {

std::stringstream outputStream;

void flush() {
    std::cout << outputStream.rdbuf();
    std::cout << std::flush;
}
void loadToCout() {
    std::cout << outputStream.rdbuf();
}

}; // namespace DimEngineZ::logger