#include "logger.h"

namespace encfs
{
#define LOG_FILEPATH "/root/encfs.log"

using std::endl;
using std::fstream;
using std::string;

Logger::Logger(void)
    : fs_(LOG_FILEPATH, fstream::in | fstream::out | fstream::trunc)
{
}

void Logger::WriteLog(const string &text)
{
    if (fs_.is_open()) {
        fs_ << text << endl;
    }
}

}  // namespace encfs
