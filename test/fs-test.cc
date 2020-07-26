#include <cstring>
#include <fstream>
#include <iostream>
#include <string>

#define TEST_FILE "test.txt"
#define TEST_STR "Hello World"

using namespace std;


int main(int argc, char *argv[])
{
    string filename;
    fstream fs;
    if (2 > argc || NULL == argv[1]) {
        cout << "Usage: fs-test <mount point>" << endl;
        exit(1);
    }

    filename = string(argv[1]) + "/" + TEST_FILE;

    fs.open(filename, fstream::in | fstream::out | fstream::trunc);
    if (!fs.is_open()) {
        cout << "Failed to open file " << filename << "with err " << errno
             << endl;
        exit(1);
    }

    fs.write(TEST_STR, strlen(TEST_STR));

    fs.close();

    return 0;
}
