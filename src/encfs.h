#ifndef ENCFS_H_
#define ENCFS_H_

#include <fuse.h>
#include <string>

#include "logger.h"

#define ENCFS ((encfs::Encfs *) fuse_get_context()->private_data)

namespace encfs
{
class Encfs
{
    std::string rootdir_;
    Logger log_;

public:
    Encfs(const std::string &rootdir);
    ~Encfs() = default;

    Encfs(const Encfs &) = delete;
    Encfs(Encfs &&) = delete;
    Encfs &operator=(Encfs) = delete;

    std::string GetRealpath(const std::string &fpath) const;
    void WriteLog(const std::string &text);
};

int encfs_getattr(const char *fpath, struct stat *statbuf);
int encfs_mknod(const char *fpath, mode_t mode, dev_t dev);
int encfs_open(const char *fpath, struct fuse_file_info *fi);
int encfs_read(const char *fpath,
               char *buf,
               size_t size,
               off_t offset,
               struct fuse_file_info *fi);
int encfs_write(const char *fpath,
                const char *buf,
                size_t size,
                off_t offset,
                struct fuse_file_info *fi);
int encfs_release(const char *fpath, struct fuse_file_info *fi);
int encfs_unlink(const char *fpath);
int encfs_truncate(const char *fpath, off_t size);

}  // namespace encfs

#endif /* ENCFS_H_ */
