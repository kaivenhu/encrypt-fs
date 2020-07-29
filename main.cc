#include <cstring>
#include <iostream>
#include <memory>

#include <fuse.h>

#include "src/encfs.h"

static void init_ops(struct fuse_operations *op)
{
    memset(op, 0, sizeof(struct fuse_operations));
    op->getattr = encfs::encfs_getattr;
    op->mknod = encfs::encfs_mknod;
    op->open = encfs::encfs_open;
    op->read = encfs::encfs_read;
    op->write = encfs::encfs_write;
    op->release = encfs::encfs_release;
    op->unlink = encfs::encfs_unlink;
    op->truncate = encfs::encfs_truncate;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    fuse_operations encfs_ops;

    if (3 > argc || !argv[argc - 2] || !argv[argc - 1]) {
        std::cout << "usage: encfs <fuse options> <root dir> <mount point>"
                  << std::endl;
        exit(1);
    }

    auto fs = std::make_unique<encfs::Encfs>(argv[argc - 2]);
    argv[argc - 2] = argv[argc - 1];
    argv[argc - 1] = NULL;
    --argc;

    std::cout << "Hell World !" << std::endl;
    init_ops(&encfs_ops);

    fs->WriteLog("Hello World !");

    ret = fuse_main(argc, argv, &encfs_ops, fs.get());

    fs->WriteLog("Bye~");

    return ret;
}
