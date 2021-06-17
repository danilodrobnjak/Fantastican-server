#ifndef _LOGGER_H_
#define _LOGGER_H_
#include<fstream>
#include<iostream>
#include <cstdarg>
#include<thread>
#include<string>

//Unikat
class Log {

    Log(const Log&) {};             // copy constructor is private
    Log& operator=(const Log&) { return *this; };  // assignment operator is private

    static const std::string m_sFileName;
    /**
    *   Singleton logger class object pointer.
    **/
    static Log* m_pThis;
    /**
    *   Log file stream object.
    **/
    static std::ofstream m_Logfile;
public:
    Log();
    void Logg(const std::string& sMessage);
    void Logg(const char* format, ...);
    Log& operator<<(const std::string& sMessage);
    static Log* GetLogger();
    void Log::Logg(std::thread::id& this_id);
    void Log::Logg(int br);

};
#endif // !_LOGGER_H_
