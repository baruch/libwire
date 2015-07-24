#ifndef WIRE_LIB_IO_GEN_H
#define WIRE_LIB_IO_GEN_H

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <sys/vfs.h>
#include <sys/ioctl.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <unistd.h>
#include <netdb.h>
#include <ifaddrs.h>
#include <sys/uio.h>
#include <sys/mman.h>
#include <dirent.h>
#include <glob.h>
#include <stdio.h>
#include <ftw.h>
typedef int (*glob_errfunc_t)(const char *epath, int eerrno) ;
typedef int (*ftw_cb_t)(const char *fpath, const struct stat *sb, int typeflag) ;
typedef int (*nftw_cb_t)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf) ;
int wio_open(const char *pathname, int flags, mode_t mode);
int wio_close(int fd);
ssize_t wio_pread(int fd, void *buf, size_t count, off_t offset);
ssize_t wio_pwrite(int fd, const void *buf, size_t count, off_t offset);
ssize_t wio_read(int fd, void *buf, size_t count);
ssize_t wio_write(int fd, const void *buf, size_t count);
int wio_fstat(int fd, struct stat *buf);
int wio_stat(const char *path, struct stat *buf);
int wio_ftruncate(int fd, off_t length);
int wio_fallocate(int fd, int mode, off_t offset, off_t len);
int wio_fsync(int fd);
int wio_statfs(const char *path, struct statfs *buf);
int wio_fstatfs(int fd, struct statfs *buf);
int wio_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int wio_getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags);
int wio_ioctl(int d, unsigned long request, void *argp);
int wio_getifaddrs(struct ifaddrs **ifap);
ssize_t wio_readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t wio_writev(int fd, const struct iovec *iov, int iovcnt);
ssize_t wio_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t wio_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
void * wio_mmap(void *addr, size_t length, int protc, int flags, int fd, off_t offset);
int wio_munmap(void *addr, size_t length);
DIR * wio_opendir(const char *name);
DIR * wio_fdopendir(int fd);
int wio_closedir(DIR *dirp);
int wio_readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
struct dirent * wio_readdir(DIR *dirp);
int wio_read_file_content(const char *filename, char *buf, size_t bufsz);
int wio_glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob);
void wio_globfree(glob_t *pglob);
FILE * wio_popen(const char *command, const char *type);
int wio_pclose(FILE *stream);
int wio_fgetc(FILE *stream);
char * wio_fgets(char *s, int size, FILE *stream);
int wio_spawn(char **args, int *stdin_fd, int *stdout_fd, int *stderr_fd);
int wio_kill(pid_t pid, int sig);
int wio_ftw(const char *dirpath, ftw_cb_t cb, int nopenfd);
int wio_nftw(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags);
pid_t wio_wait(int *status);
pid_t wio_waitpid(pid_t pid, int *status, int options);
int wio_dup(int oldfd);
int wio_dup2(int oldfd, int newfd);

#endif
