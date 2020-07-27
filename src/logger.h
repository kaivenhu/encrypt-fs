#ifndef LOGGER_H_
#define LOGGER_H_

#include <fstream>
#include <string>

namespace encfs
{
class Logger
{
    std::fstream fs_;

public:
    Logger(void);
    ~Logger() = default;
    Logger(const Logger &) = delete;
    Logger(Logger &&) = delete;
    Logger &operator=(Logger) = delete;

    void WriteLog(const std::string &text);
};

}  // namespace encfs

#define LOGGER ((encfs::Logger *) fuse_get_context()->private_data)


#endif /* LOGGER_H_ */
