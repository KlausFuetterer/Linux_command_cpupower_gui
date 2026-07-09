#include "get_various_info.h"
#include <fstream>
#include <thread>
#include <QMessageBox>

Get_various_Info::Get_various_Info() {}
Get_various_Info::~Get_various_Info() {}

std::basic_string<char> Get_various_Info::getCompilerInfo()
{
    std::basic_string<char> compiler;

#if defined(__ANDROID__)
#if defined(__clang__)
    compiler = "Android Clang " + std::to_string(__clang_major__) + "." +
               std::to_string(__clang_minor__) + "." +
               std::to_string(__clang_patchlevel__);
#else
    compiler = "Unknown Android compiler";
#endif
#elif defined(__GNUC__)
    compiler = "GCC " + std::to_string(__GNUC__) + "." +
               std::to_string(__GNUC_MINOR__) + "." +
               std::to_string(__GNUC_PATCHLEVEL__);
#elif defined(__clang__)
    compiler = "Clang " + std::to_string(__clang_major__) + "." +
               std::to_string(__clang_minor__) + "." +
               std::to_string(__clang_patchlevel__);
#elif defined(_MSC_VER)
    compiler = "MSVC " + std::to_string(_MSC_VER);
#else
    compiler = "Unknown compiler";
#endif

    return compiler;
}

std::basic_string<char> Get_various_Info::getCpuModel()
{
    std::ifstream cpuInfoFile("/proc/cpuinfo");
    if (!cpuInfoFile.is_open()) {
        // std::cerr << "Error: Unable to open /proc/cpuinfo" << std::endl;
        QMessageBox::warning(nullptr, "Title: Error: Unable to open /proc/cpuinfo",
                             "Text: Error: Unable to open /proc/cpuinfo",
                             QMessageBox::StandardButton(QMessageBox::Ok),
                             QMessageBox::StandardButton(QMessageBox::NoButton));
        return "Error: Unable to open /proc/cpuinfo";
    }

    std::string line;
    while (std::getline(cpuInfoFile, line)) {
        // Check if the line contains "model name" or "Processor"
        if (line.find("model name") != std::string::npos || line.find("Processor") != std::string::npos) {
            // Split the line at the colon and return the model name
            size_t colonPos = line.find(':');
            if (colonPos != std::string::npos) {
                std::string modelName = line.substr(colonPos + 1);
                return modelName.erase(0, modelName.find_first_not_of(" \t")); // Trim leading whitespace
            }
        }
    }
    return "Unknown CPU Model";
}

std::basic_string<char> Get_various_Info::getConcurrentThreadsSupported()
{
    return std::to_string(std::thread::hardware_concurrency());
}
