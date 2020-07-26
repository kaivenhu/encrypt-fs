#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include "encfs.h"

#define UNUSED [[maybe_unused]]

int encfs_getattr(const char *fpath, struct stat *statbuf)
{
    return lstat(fpath, statbuf);
}

int encfs_mknod(const char *fpath, mode_t mode, dev_t dev)
{
    return mknod(fpath, mode, dev);
}

int encfs_open(const char *fpath, struct fuse_file_info *fi)
{
    return open(fpath, fi->flags);
}

int encfs_read(UNUSED const char *fpath,
               char *buf,
               size_t size,
               off_t offset,
               struct fuse_file_info *fi)
{
    return pread(fi->fh, buf, size, offset);
}

int encfs_write(UNUSED const char *fpath,
                const char *buf,
                size_t size,
                off_t offset,
                struct fuse_file_info *fi)
{
    return pwrite(fi->fh, buf, size, offset);
}
