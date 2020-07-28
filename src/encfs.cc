#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>

#include "encfs.h"
#include "logger.h"

#define UNUSED [[maybe_unused]]

using std::string;

int encfs_getattr(const char *fpath, struct stat *statbuf)
{
    LOGGER->WriteLog("getattr: " + string(fpath));
    if (0 != lstat(fpath, statbuf)) {
        return -errno;
    }
    return 0;
}

int encfs_mknod(const char *fpath, mode_t mode, dev_t dev)
{
    LOGGER->WriteLog("mknod: " + string(fpath));
    if (0 != mknod(fpath, mode, dev)) {
        return -errno;
    }
    return 0;
}

int encfs_open(const char *fpath, struct fuse_file_info *fi)
{
    int fd = -1;

    LOGGER->WriteLog("open: " + string(fpath));

    if (-1 == (fd = open(fpath, fi->flags))) {
        return -errno;
    }
    fi->fh = fd;
    return 0;
}

int encfs_read(UNUSED const char *fpath,
               char *buf,
               size_t size,
               off_t offset,
               struct fuse_file_info *fi)
{
    ssize_t ret = -1;
    LOGGER->WriteLog("read: " + string(fpath));

    if (-1 == (ret = pread(fi->fh, buf, size, offset))) {
        return -errno;
    }
    return ret;
}

int encfs_write(UNUSED const char *fpath,
                const char *buf,
                size_t size,
                off_t offset,
                struct fuse_file_info *fi)
{
    ssize_t ret = -1;
    LOGGER->WriteLog("write: " + string(fpath));

    if (-1 == (ret = pwrite(fi->fh, buf, size, offset))) {
        return -errno;
    }
    return ret;
}
