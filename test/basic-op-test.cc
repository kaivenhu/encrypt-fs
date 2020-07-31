#include <cstring>
#include <iostream>
#include <string>

#include <fcntl.h>
#include <unistd.h>

#define TEST_FILE "test.txt"

using namespace std;

#define checker(ret, key)                                          \
    do {                                                           \
        if (!(ret)) {                                              \
            cout << "Failed to do " key ", err:" << errno << endl; \
            exit(1);                                               \
        }                                                          \
    } while (0);


int main(int argc, char *argv[])
{
    ssize_t ret;
    string filename;
    char buf[32] = {'\0'};
    string read_txt;
    string write_txt;
    int fd = -1;

    if (2 > argc || NULL == argv[1]) {
        cout << "Usage: fs-test <mount point>" << endl;
        exit(1);
    }

    filename = string(argv[1]) + "/" + TEST_FILE;

    checker(-1 != (fd = open(filename.c_str(), O_WRONLY | O_CREAT)), "open");

    for (int i = 0; i < 8; ++i) {
        string tmp;
        for (int j = 0; j < 3000; ++j)
            tmp += to_string(i);
        tmp + ", ";
        checker(-1 != write(fd, tmp.c_str(), tmp.length()), "write");
        write_txt += tmp;
    }
    checker(-1 != close(fd), "close");

    checker(-1 != (fd = open(filename.c_str(), O_RDONLY)), "open");

    while (0 < (ret = read(fd, buf, sizeof(buf) - 1))) {
        buf[ret] = '\0';
        read_txt += string(buf);
    }
    checker(-1 != close(fd), "close");

    if (read_txt != write_txt) {
        cout << "Failed to read buf " << read_txt << "." << endl;
        exit(1);
    }
    checker(-1 != unlink(filename.c_str()), "unlink");

    return 0;
}
