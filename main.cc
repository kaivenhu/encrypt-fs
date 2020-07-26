#include <cstring>
#include <iostream>

#include <fuse.h>

#include "src/encfs.h"

static void init_ops(struct fuse_operations *op)
{
    memset(op, 0, sizeof(struct fuse_operations));
    op->getattr = encfs_getattr;
    op->mknod = encfs_mknod;
    op->open = encfs_open;
    op->read = encfs_read;
    op->write = encfs_write;
}

int main(int argc, char *argv[])
{
    fuse_operations encfs_ops;
    std::cout << "Hell World !" << std::endl;
    init_ops(&encfs_ops);

    return fuse_main(argc, argv, &encfs_ops, NULL);
}
