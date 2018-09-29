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
int wio_creat(const char* filename, mode_t mode);
int wio_creat64(const char* filename, mode_t mode);
int wio_open(const char *pathname, int flags, mode_t mode);
int wio_close(int fd);
ssize_t wio_pread(int fd, void *buf, size_t count, off_t offset);
ssize_t wio_pwrite(int fd, const void *buf, size_t count, off_t offset);
ssize_t wio_read(int fd, void *buf, size_t count);
ssize_t wio_write(int fd, const void *buf, size_t count);
int wio_lockf(int fd, int cmd, off_t len);
int wio_lockf64(int fd, int cmd, off64_t len);
int wio_posix_fadvise(int fd, off_t offset, off_t len, int advise);
int wio_posix_fadvise64(int fd, off64_t offset, off64_t len, int advise);
int wio_posix_fallocate(int fd, off_t offset, off_t len);
int wio_posix_fallocate64(int fd, off64_t offset, off64_t len);
int wio_fstat(int fd, struct stat *buf);
int wio_stat(const char *path, struct stat *buf);
off_t wio_lseek(int fd, off_t offset, int whence);
off64_t wio_lseek64(int fd, off64_t offset, int whence);
int wio_ftruncate(int fd, off_t length);
int wio_truncate(const char* filename, off_t offset);
int wio_fallocate(int fd, int mode, off_t offset, off_t len);
int wio_fsync(int fd);
void wio_sync(void);
int wio_fdatasync(int fd);
int wio_statfs(const char *path, struct statfs *buf);
int wio_fstatfs(int fd, struct statfs *buf);
int wio_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
int wio_getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags);
int wio_ioctl(int d, unsigned long request, void *argp);
int wio_getifaddrs(struct ifaddrs **ifap);
ssize_t wio_readv(int fd, const struct iovec *iov, int iovcnt);
ssize_t wio_writev(int fd, const struct iovec *iov, int iovcnt);
ssize_t wio_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset);
ssize_t wio_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset);
int wio_msync(void* addr, size_t len, int flags);
int wio_madvise(void* addr, size_t len, int advice);
int wio_posix_madvise(void* addr, size_t len, int advice);
int wio_mlock(const void* addr, size_t len);
int wio_munlock(const void* addr, size_t len);
int wio_mlockall(int flags);
int wio_munlockall(void);
DIR * wio_opendir(const char *name);
DIR * wio_fdopendir(int fd);
int wio_closedir(DIR *dirp);
int wio_readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result);
struct dirent * wio_readdir(DIR *dirp);
void wio_rewinddir(DIR* dirp);
void wio_seekdir(DIR* dirp, long int pos);
long int wio_telldir(DIR* dirp);
int wio_dirfd(DIR* dirp);
int wio_glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob);
void wio_globfree(glob_t *pglob);
FILE * wio_popen(const char *command, const char *type);
int wio_pclose(FILE *stream);
int wio_fgetc(FILE *stream);
char * wio_fgets(char *s, int size, FILE *stream);
int wio_kill(pid_t pid, int sig);
int wio_ftw(const char *dirpath, ftw_cb_t cb, int nopenfd);
int wio_nftw(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags);
pid_t wio_wait(int *status);
pid_t wio_waitpid(pid_t pid, int *status, int options);
pid_t wio_wait3(int* stat_loc, int options, struct rusage* usage);
pid_t wio_wait4(pid_t pid, int* stat_loc, int options, struct rusage* usage);
int wio_dup(int oldfd);
int wio_dup2(int oldfd, int newfd);
unsigned int wio_alarm(unsigned int seconds);
unsigned int wio_ualarm(unsigned value, unsigned interval);
int wio_chown(const char *filename, uid_t owner, gid_t group);
int wio_fchown(int fd, uid_t owner, gid_t group);
int wio_lchown(const char* filename, uid_t owner, gid_t group);
int wio_fchownat(int fd, const char* filename, uid_t owner, gid_t group, int flag);
int wio_fchdir(int fd);
char* wio_getcwd(char* buf, size_t size);
int wio_execve(const char* path, char *const* argv, char *const* envp);
int wio_execv(const char* path, char *const* argv);
int wio_execvpe(const char* file, char *const* argv, char *const* envp);
pid_t wio_getpid(void);
pid_t wio_getppid(void);
pid_t wio_getpgrp(void);
pid_t wio_getpgid(pid_t pid);
int wio_setpgid(pid_t pid, pid_t pgid);
int wio_setpgrp(void);
pid_t wio_setsid(void);
pid_t wio_getsid(pid_t pid);
uid_t wio_getuid(void);
uid_t wio_geteuid(void);
gid_t wio_getgid(void);
pid_t wio_fork(void);
int wio_link(const char* from, const char* to);
int wio_symlink(const char* from, const char* to);
ssize_t wio_readlink(const char* path, char* buf, size_t len);
int wio_unlink(const char *name);
int wio_rmdir(const char* path);

#endif
