#include <cstring>
#include <iostream>
#include <memory>

#include <fuse.h>

#include "src/encfs.h"
#include "src/logger.h"

static void init_ops(struct fuse_operations *op)
{
    memset(op, 0, sizeof(struct fuse_operations));
    op->getattr = encfs_getattr;
    op->mknod = encfs_mknod;
    op->open = encfs_open;
    op->read = encfs_read;
    op->write = encfs_write;
    op->release = encfs_release;
    op->unlink = encfs_unlink;
    op->truncate = encfs_truncate;
}

int main(int argc, char *argv[])
{
    int ret = 0;
    fuse_operations encfs_ops;
    auto logger = std::make_unique<encfs::Logger>();

    std::cout << "Hell World !" << std::endl;
    init_ops(&encfs_ops);

    logger->WriteLog("Hello World !");

    ret = fuse_main(argc, argv, &encfs_ops, logger.get());

    logger->WriteLog("Bye~");

    return ret;
}
