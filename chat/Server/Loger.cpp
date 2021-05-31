#include "Log.h"
#include"Util.h"
const std::string Log::m_sFileName = "Log.txt";
Log* Log::m_pThis = NULL;
std::ofstream Log::m_Logfile;
Log::Log() //default constr
{

}
Log* Log::GetLogger() {
    if (m_pThis == NULL) {
        m_pThis = new Log();
        m_Logfile.open(m_sFileName.c_str(), std::ios::out | std::ios::app);
    }
    return m_pThis;
}

void Log::Logg(const char* format, ...)
{
    char* sMessage = NULL;
    int nLength = 0;
    va_list args;
    va_start(args, format);
    //  Return the number of characters in the string referenced the list of arguments.
    // _vscprintf doesn't count terminating '\0' (that's why +1)
    nLength = _vscprintf(format, args) + 1;
    sMessage = new char[nLength];
    vsprintf_s(sMessage, nLength, format, args);
    //vsprintf(sMessage, format, args);
    m_Logfile << Util::CurrentDateTime() << ":\t";
    m_Logfile << sMessage << "\n";
    va_end(args);

    delete[] sMessage;
}

void Log::Logg(const std::string& sMessage)
{
    m_Logfile << Util::CurrentDateTime() << ":\t";
    m_Logfile << sMessage << "\n";
}
void Log::Logg(int br)
{
    m_Logfile << Util::CurrentDateTime() << ":\t";
    m_Logfile << br << "\n";
}

void Log::Logg(std::thread::id &this_id)
{
    m_Logfile << Util::CurrentDateTime() << ":\t";
    m_Logfile << this_id << "\n";
}

Log& Log::operator<<(const std::string& sMessage)
{
    m_Logfile << "\n" << Util::CurrentDateTime() << ":\t";
    m_Logfile << sMessage << "\n";
    return *this;
}