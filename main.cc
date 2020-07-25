#include <iostream>

#include <fuse.h>

static struct fuse_operations operations = {};

int main(int argc, char *argv[])
{
    std::cout << "Hell World !" << std::endl;

    return fuse_main(argc, argv, &operations, NULL);
}
