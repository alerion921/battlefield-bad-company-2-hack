#include "main.h"
#include <sstream>
#include <fstream>
#include <string>
#include <ctime>
#include <chrono>


static std::string stripFilePath(const std::string &path)
{
    return path.substr(path.find_last_of('\\')+1);
}


class FormatStr
{
public:
    FormatStr(const char *format, va_list vl)
    {
        int size = vsnprintf(nullptr, 0, format, vl);
        m_str = new char[size+1];
        vsnprintf(m_str, size+1, format, vl);
    }
    ~FormatStr()
    {
        delete[] m_str;
    }
    const char *str() const
    {
        return m_str;
    }
private:
    char *m_str;
};


void WriteToLogfile(const char *file, const char *func, int line, const char *desc)
{
    char path[MAX_PATH];
    GetModuleFileName(hl::GetCurrentModule(), path, MAX_PATH);
    std::string filename(path);
    filename += "_log.txt";

    char strtime[256];
    auto timept = std::chrono::system_clock::now();
    auto timetp = std::chrono::system_clock::to_time_t(timept);
    strftime(strtime, sizeof(strtime), "%X", std::localtime(&timetp));

    std::ofstream logfile(filename, std::ios::out|std::ios::app);
    logfile << strtime;
    logfile << " [" << stripFilePath(file);
    logfile << "|" << func;
    logfile << "|" << line;
    logfile << "] " << desc;
}


void LogError(const char *file, const char *func, int line, const char *format, ...)
{
    va_list vl;
    va_start(vl, format);
    FormatStr desc(format, vl);

    GetMain()->getHack()->getCon()->printf("ERROR: [%s|%s|%i][%08X] %s", stripFilePath(file).c_str(), func, line, GetLastError(), desc.str());

    WriteToLogfile(file, func, line, desc.str());

    va_end(vl);
}

void LogError(const char *format, ...)
{
    va_list vl;
    va_start(vl, format);
    FormatStr desc(format, vl);

    GetMain()->getHack()->getCon()->printf("ERROR: [%08X] %s", GetLastError(), desc.str());

    va_end(vl);
}


void LogDebug(const char *file, const char *func, int line, const char *format, ...)
{
    va_list vl;
    va_start(vl, format);
    FormatStr desc(format, vl);

    GetMain()->getHack()->getCon()->vprintf(format, vl);

    WriteToLogfile(file, func, line, desc.str());

    va_end(vl);
}