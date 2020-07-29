#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <cerrno>
#include <cstring>
#include <string>

#include "encfs.h"

#define UNUSED [[maybe_unused]]


namespace encfs
{
using std::string;

Encfs::Encfs(const string &rootdir) : rootdir_(rootdir), log_() {}

string Encfs::GetRealpath(const string &fpath) const
{
    return rootdir_ + "/" + fpath;
}

void Encfs::WriteLog(const string &text)
{
    log_.WriteLog(text);
}

int encfs_getattr(const char *fpath, struct stat *statbuf)
{
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("getattr: " + realpath);
    if (0 != lstat(realpath.c_str(), statbuf)) {
        return -errno;
    }
    return 0;
}

int encfs_mknod(const char *fpath, mode_t mode, dev_t dev)
{
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("mknod: " + realpath);
    if (0 != mknod(realpath.c_str(), mode, dev)) {
        return -errno;
    }
    return 0;
}

int encfs_open(const char *fpath, struct fuse_file_info *fi)
{
    int fd = -1;
    const string realpath = ENCFS->GetRealpath(fpath);

    ENCFS->WriteLog("open: " + realpath);

    if (-1 == (fd = open(realpath.c_str(), fi->flags))) {
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
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("read: " + realpath);

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
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("write: " + realpath);

    if (-1 == (ret = pwrite(fi->fh, buf, size, offset))) {
        return -errno;
    }
    return ret;
}

int encfs_release(const char *fpath, struct fuse_file_info *fi)
{
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("release: " + realpath);

    if (0 != close(fi->fh)) {
        return -errno;
    }
    return 0;
}

int encfs_unlink(const char *fpath)
{
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("unlink: " + realpath);

    if (0 != unlink(realpath.c_str())) {
        return -errno;
    }
    return 0;
}

int encfs_truncate(const char *fpath, off_t size)
{
    const string realpath = ENCFS->GetRealpath(fpath);
    ENCFS->WriteLog("truncate: " + realpath);

    if (0 != truncate(realpath.c_str(), size)) {
        return -errno;
    }
    return 0;
}


}  // namespace encfs
