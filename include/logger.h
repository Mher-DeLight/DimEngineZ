#pragma once
#include "common.h"
#include <iostream>
#include <sstream>

namespace DimEngineZ::logger {

void log(std::string_view message);
void logLine(std::string_view line);

void flushLog(std::string_view message);
void flushLogLine(std::string_view line);

void flush();
void loadToCout();

}; // namespace DimEngineZ::logger