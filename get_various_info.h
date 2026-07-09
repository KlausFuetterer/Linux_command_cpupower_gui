#ifndef GET_VARIOUS_INFO_H
#define GET_VARIOUS_INFO_H

#include <string>

struct Get_various_Info
{
    Get_various_Info();
    ~Get_various_Info();
    static std::basic_string<char> getCompilerInfo();
    static std::basic_string<char> getCpuModel();
    static std::basic_string<char> getConcurrentThreadsSupported();
};
#endif // GET_VARIOUS_INFO_H
