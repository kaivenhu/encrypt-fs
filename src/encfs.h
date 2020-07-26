#ifndef ENCFS_H_
#define ENCFS_H_

#include <fuse.h>

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

#endif /* ENCFS_H_ */
