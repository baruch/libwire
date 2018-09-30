#include "wire_io_gen.h"

enum wio_type {
	IO_CREAT,
	IO_CREAT64,
	IO_OPEN,
	IO_CLOSE,
	IO_PREAD,
	IO_PWRITE,
	IO_READ,
	IO_WRITE,
	IO_LOCKF,
	IO_LOCKF64,
	IO_POSIX_FADVISE,
	IO_POSIX_FADVISE64,
	IO_POSIX_FALLOCATE,
	IO_POSIX_FALLOCATE64,
	IO_FSTAT,
	IO_STAT,
	IO_LSEEK,
	IO_LSEEK64,
	IO_FTRUNCATE,
	IO_TRUNCATE,
	IO_FALLOCATE,
	IO_FSYNC,
	IO_SYNC,
	IO_FDATASYNC,
	IO_STATFS,
	IO_FSTATFS,
	IO_GETADDRINFO,
	IO_GETNAMEINFO,
	IO_IOCTL,
	IO_GETIFADDRS,
	IO_READV,
	IO_WRITEV,
	IO_PREADV,
	IO_PWRITEV,
	IO_MSYNC,
	IO_MADVISE,
	IO_POSIX_MADVISE,
	IO_MLOCK,
	IO_MUNLOCK,
	IO_MLOCKALL,
	IO_MUNLOCKALL,
	IO_OPENDIR,
	IO_FDOPENDIR,
	IO_CLOSEDIR,
	IO_READDIR_R,
	IO_READDIR,
	IO_REWINDDIR,
	IO_SEEKDIR,
	IO_TELLDIR,
	IO_DIRFD,
	IO_GLOB,
	IO_GLOBFREE,
	IO_POPEN,
	IO_PCLOSE,
	IO_FGETC,
	IO_FGETS,
	IO_KILL,
	IO_FTW,
	IO_NFTW,
	IO_WAIT,
	IO_WAITPID,
	IO_WAIT3,
	IO_WAIT4,
	IO_DUP,
	IO_DUP2,
	IO_ALARM,
	IO_UALARM,
	IO_CHOWN,
	IO_FCHOWN,
	IO_LCHOWN,
	IO_FCHOWNAT,
	IO_FCHDIR,
	IO_GETCWD,
	IO_EXECVE,
	IO_EXECV,
	IO_EXECVPE,
	IO_GETPID,
	IO_GETPPID,
	IO_GETPGRP,
	IO_GETPGID,
	IO_SETPGID,
	IO_SETPGRP,
	IO_SETSID,
	IO_GETSID,
	IO_GETUID,
	IO_GETEUID,
	IO_GETGID,
	IO_FORK,
	IO_LINK,
	IO_SYMLINK,
	IO_READLINK,
	IO_UNLINK,
	IO_RMDIR,
};

struct wire_io_act {
    struct wire_io_act_common common;
    enum wio_type type;
    union {
        struct {
            const char* filename;
            mode_t mode;
            int ret;
            int verrno;
        } creat;
        struct {
            const char* filename;
            mode_t mode;
            int ret;
            int verrno;
        } creat64;
        struct {
            const char * pathname;
            int flags;
            mode_t mode;
            int ret;
            int verrno;
        } open;
        struct {
            int fd;
            int ret;
            int verrno;
        } close;
        struct {
            int fd;
            void * buf;
            size_t count;
            off_t offset;
            ssize_t ret;
            int verrno;
        } pread;
        struct {
            int fd;
            const void * buf;
            size_t count;
            off_t offset;
            ssize_t ret;
            int verrno;
        } pwrite;
        struct {
            int fd;
            void * buf;
            size_t count;
            ssize_t ret;
            int verrno;
        } read;
        struct {
            int fd;
            const void * buf;
            size_t count;
            ssize_t ret;
            int verrno;
        } write;
        struct {
            int fd;
            int cmd;
            off_t len;
            int ret;
            int verrno;
        } lockf;
        struct {
            int fd;
            int cmd;
            off64_t len;
            int ret;
            int verrno;
        } lockf64;
        struct {
            int fd;
            off_t offset;
            off_t len;
            int advise;
            int ret;
            int verrno;
        } posix_fadvise;
        struct {
            int fd;
            off64_t offset;
            off64_t len;
            int advise;
            int ret;
            int verrno;
        } posix_fadvise64;
        struct {
            int fd;
            off_t offset;
            off_t len;
            int ret;
            int verrno;
        } posix_fallocate;
        struct {
            int fd;
            off64_t offset;
            off64_t len;
            int ret;
            int verrno;
        } posix_fallocate64;
        struct {
            int fd;
            struct stat * buf;
            int ret;
            int verrno;
        } fstat;
        struct {
            const char * path;
            struct stat * buf;
            int ret;
            int verrno;
        } stat;
        struct {
            int fd;
            off_t offset;
            int whence;
            off_t ret;
            int verrno;
        } lseek;
        struct {
            int fd;
            off64_t offset;
            int whence;
            off64_t ret;
            int verrno;
        } lseek64;
        struct {
            int fd;
            off_t length;
            int ret;
            int verrno;
        } ftruncate;
        struct {
            const char* filename;
            off_t offset;
            int ret;
            int verrno;
        } truncate;
        struct {
            int fd;
            int mode;
            off_t offset;
            off_t len;
            int ret;
            int verrno;
        } fallocate;
        struct {
            int fd;
            int ret;
            int verrno;
        } fsync;
        struct {
        } sync;
        struct {
            int fd;
            int ret;
            int verrno;
        } fdatasync;
        struct {
            const char * path;
            struct statfs * buf;
            int ret;
            int verrno;
        } statfs;
        struct {
            int fd;
            struct statfs * buf;
            int ret;
            int verrno;
        } fstatfs;
        struct {
            const char * node;
            const char * service;
            const struct addrinfo * hints;
            struct addrinfo ** res;
            int ret;
            int verrno;
        } getaddrinfo;
        struct {
            const struct sockaddr * sa;
            socklen_t salen;
            char * host;
            socklen_t hostlen;
            char * serv;
            socklen_t servlen;
            int flags;
            int ret;
            int verrno;
        } getnameinfo;
        struct {
            int d;
            unsigned long request;
            void * argp;
            int ret;
            int verrno;
        } ioctl;
        struct {
            struct ifaddrs ** ifap;
            int ret;
            int verrno;
        } getifaddrs;
        struct {
            int fd;
            const struct iovec * iov;
            int iovcnt;
            ssize_t ret;
            int verrno;
        } readv;
        struct {
            int fd;
            const struct iovec * iov;
            int iovcnt;
            ssize_t ret;
            int verrno;
        } writev;
        struct {
            int fd;
            const struct iovec * iov;
            int iovcnt;
            off_t offset;
            ssize_t ret;
            int verrno;
        } preadv;
        struct {
            int fd;
            const struct iovec * iov;
            int iovcnt;
            off_t offset;
            ssize_t ret;
            int verrno;
        } pwritev;
        struct {
            void* addr;
            size_t len;
            int flags;
            int ret;
            int verrno;
        } msync;
        struct {
            void* addr;
            size_t len;
            int advice;
            int ret;
            int verrno;
        } madvise;
        struct {
            void* addr;
            size_t len;
            int advice;
            int ret;
            int verrno;
        } posix_madvise;
        struct {
            const void* addr;
            size_t len;
            int ret;
            int verrno;
        } mlock;
        struct {
            const void* addr;
            size_t len;
            int ret;
            int verrno;
        } munlock;
        struct {
            int flags;
            int ret;
            int verrno;
        } mlockall;
        struct {
            int ret;
            int verrno;
        } munlockall;
        struct {
            const char * name;
            DIR * ret;
            int verrno;
        } opendir;
        struct {
            int fd;
            DIR * ret;
            int verrno;
        } fdopendir;
        struct {
            DIR * dirp;
            int ret;
            int verrno;
        } closedir;
        struct {
            DIR * dirp;
            struct dirent * entry;
            struct dirent ** result;
            int ret;
            int verrno;
        } readdir_r;
        struct {
            DIR * dirp;
            struct dirent * ret;
            int verrno;
        } readdir;
        struct {
            DIR* dirp;
        } rewinddir;
        struct {
            DIR* dirp;
            long int pos;
        } seekdir;
        struct {
            DIR* dirp;
            long int ret;
            int verrno;
        } telldir;
        struct {
            DIR* dirp;
            int ret;
            int verrno;
        } dirfd;
        struct {
            const char * pattern;
            int flags;
            glob_errfunc_t errfunc;
            glob_t * pglob;
            int ret;
            int verrno;
        } glob;
        struct {
            glob_t * pglob;
        } globfree;
        struct {
            const char * command;
            const char * type;
            FILE * ret;
            int verrno;
        } popen;
        struct {
            FILE * stream;
            int ret;
            int verrno;
        } pclose;
        struct {
            FILE * stream;
            int ret;
            int verrno;
        } fgetc;
        struct {
            char * s;
            int size;
            FILE * stream;
            char * ret;
            int verrno;
        } fgets;
        struct {
            pid_t pid;
            int sig;
            int ret;
            int verrno;
        } kill;
        struct {
            const char * dirpath;
            ftw_cb_t cb;
            int nopenfd;
            int ret;
            int verrno;
        } ftw;
        struct {
            const char * dirpath;
            nftw_cb_t cb;
            int nopenfd;
            int flags;
            int ret;
            int verrno;
        } nftw;
        struct {
            int * status;
            pid_t ret;
            int verrno;
        } wait;
        struct {
            pid_t pid;
            int * status;
            int options;
            pid_t ret;
            int verrno;
        } waitpid;
        struct {
            int* stat_loc;
            int options;
            struct rusage* usage;
            pid_t ret;
            int verrno;
        } wait3;
        struct {
            pid_t pid;
            int* stat_loc;
            int options;
            struct rusage* usage;
            pid_t ret;
            int verrno;
        } wait4;
        struct {
            int oldfd;
            int ret;
            int verrno;
        } dup;
        struct {
            int oldfd;
            int newfd;
            int ret;
            int verrno;
        } dup2;
        struct {
            unsigned int seconds;
            unsigned int ret;
            int verrno;
        } alarm;
        struct {
            unsigned value;
            unsigned interval;
            unsigned int ret;
            int verrno;
        } ualarm;
        struct {
            const char * filename;
            uid_t owner;
            gid_t group;
            int ret;
            int verrno;
        } chown;
        struct {
            int fd;
            uid_t owner;
            gid_t group;
            int ret;
            int verrno;
        } fchown;
        struct {
            const char* filename;
            uid_t owner;
            gid_t group;
            int ret;
            int verrno;
        } lchown;
        struct {
            int fd;
            const char* filename;
            uid_t owner;
            gid_t group;
            int flag;
            int ret;
            int verrno;
        } fchownat;
        struct {
            int fd;
            int ret;
            int verrno;
        } fchdir;
        struct {
            char* buf;
            size_t size;
            char* ret;
            int verrno;
        } getcwd;
        struct {
            const char* path;
            char *const* argv;
            char *const* envp;
            int ret;
            int verrno;
        } execve;
        struct {
            const char* path;
            char *const* argv;
            int ret;
            int verrno;
        } execv;
        struct {
            const char* file;
            char *const* argv;
            char *const* envp;
            int ret;
            int verrno;
        } execvpe;
        struct {
            pid_t ret;
            int verrno;
        } getpid;
        struct {
            pid_t ret;
            int verrno;
        } getppid;
        struct {
            pid_t ret;
            int verrno;
        } getpgrp;
        struct {
            pid_t pid;
            pid_t ret;
            int verrno;
        } getpgid;
        struct {
            pid_t pid;
            pid_t pgid;
            int ret;
            int verrno;
        } setpgid;
        struct {
            int ret;
            int verrno;
        } setpgrp;
        struct {
            pid_t ret;
            int verrno;
        } setsid;
        struct {
            pid_t pid;
            pid_t ret;
            int verrno;
        } getsid;
        struct {
            uid_t ret;
            int verrno;
        } getuid;
        struct {
            uid_t ret;
            int verrno;
        } geteuid;
        struct {
            gid_t ret;
            int verrno;
        } getgid;
        struct {
            pid_t ret;
            int verrno;
        } fork;
        struct {
            const char* from;
            const char* to;
            int ret;
            int verrno;
        } link;
        struct {
            const char* from;
            const char* to;
            int ret;
            int verrno;
        } symlink;
        struct {
            const char* path;
            char* buf;
            size_t len;
            ssize_t ret;
            int verrno;
        } readlink;
        struct {
            const char * name;
            int ret;
            int verrno;
        } unlink;
        struct {
            const char* path;
            int ret;
            int verrno;
        } rmdir;
    };
};

static void perform_action(struct wire_io_act *act)
{
    switch (act->type) {
        case IO_CREAT:
            act->creat.ret = creat(act->creat.filename, act->creat.mode);
            act->creat.verrno = errno;
            break;
        case IO_CREAT64:
            act->creat64.ret = creat64(act->creat64.filename, act->creat64.mode);
            act->creat64.verrno = errno;
            break;
        case IO_OPEN:
            act->open.ret = open(act->open.pathname, act->open.flags, act->open.mode);
            act->open.verrno = errno;
            break;
        case IO_CLOSE:
            act->close.ret = close(act->close.fd);
            act->close.verrno = errno;
            break;
        case IO_PREAD:
            act->pread.ret = pread(act->pread.fd, act->pread.buf, act->pread.count, act->pread.offset);
            act->pread.verrno = errno;
            break;
        case IO_PWRITE:
            act->pwrite.ret = pwrite(act->pwrite.fd, act->pwrite.buf, act->pwrite.count, act->pwrite.offset);
            act->pwrite.verrno = errno;
            break;
        case IO_READ:
            act->read.ret = read(act->read.fd, act->read.buf, act->read.count);
            act->read.verrno = errno;
            break;
        case IO_WRITE:
            act->write.ret = write(act->write.fd, act->write.buf, act->write.count);
            act->write.verrno = errno;
            break;
        case IO_LOCKF:
            act->lockf.ret = lockf(act->lockf.fd, act->lockf.cmd, act->lockf.len);
            act->lockf.verrno = errno;
            break;
        case IO_LOCKF64:
            act->lockf64.ret = lockf64(act->lockf64.fd, act->lockf64.cmd, act->lockf64.len);
            act->lockf64.verrno = errno;
            break;
        case IO_POSIX_FADVISE:
            act->posix_fadvise.ret = posix_fadvise(act->posix_fadvise.fd, act->posix_fadvise.offset, act->posix_fadvise.len, act->posix_fadvise.advise);
            act->posix_fadvise.verrno = errno;
            break;
        case IO_POSIX_FADVISE64:
            act->posix_fadvise64.ret = posix_fadvise64(act->posix_fadvise64.fd, act->posix_fadvise64.offset, act->posix_fadvise64.len, act->posix_fadvise64.advise);
            act->posix_fadvise64.verrno = errno;
            break;
        case IO_POSIX_FALLOCATE:
            act->posix_fallocate.ret = posix_fallocate(act->posix_fallocate.fd, act->posix_fallocate.offset, act->posix_fallocate.len);
            act->posix_fallocate.verrno = errno;
            break;
        case IO_POSIX_FALLOCATE64:
            act->posix_fallocate64.ret = posix_fallocate64(act->posix_fallocate64.fd, act->posix_fallocate64.offset, act->posix_fallocate64.len);
            act->posix_fallocate64.verrno = errno;
            break;
        case IO_FSTAT:
            act->fstat.ret = fstat(act->fstat.fd, act->fstat.buf);
            act->fstat.verrno = errno;
            break;
        case IO_STAT:
            act->stat.ret = stat(act->stat.path, act->stat.buf);
            act->stat.verrno = errno;
            break;
        case IO_LSEEK:
            act->lseek.ret = lseek(act->lseek.fd, act->lseek.offset, act->lseek.whence);
            act->lseek.verrno = errno;
            break;
        case IO_LSEEK64:
            act->lseek64.ret = lseek64(act->lseek64.fd, act->lseek64.offset, act->lseek64.whence);
            act->lseek64.verrno = errno;
            break;
        case IO_FTRUNCATE:
            act->ftruncate.ret = ftruncate(act->ftruncate.fd, act->ftruncate.length);
            act->ftruncate.verrno = errno;
            break;
        case IO_TRUNCATE:
            act->truncate.ret = truncate(act->truncate.filename, act->truncate.offset);
            act->truncate.verrno = errno;
            break;
        case IO_FALLOCATE:
            act->fallocate.ret = fallocate(act->fallocate.fd, act->fallocate.mode, act->fallocate.offset, act->fallocate.len);
            act->fallocate.verrno = errno;
            break;
        case IO_FSYNC:
            act->fsync.ret = fsync(act->fsync.fd);
            act->fsync.verrno = errno;
            break;
        case IO_SYNC:
            sync();
            break;
        case IO_FDATASYNC:
            act->fdatasync.ret = fdatasync(act->fdatasync.fd);
            act->fdatasync.verrno = errno;
            break;
        case IO_STATFS:
            act->statfs.ret = statfs(act->statfs.path, act->statfs.buf);
            act->statfs.verrno = errno;
            break;
        case IO_FSTATFS:
            act->fstatfs.ret = fstatfs(act->fstatfs.fd, act->fstatfs.buf);
            act->fstatfs.verrno = errno;
            break;
        case IO_GETADDRINFO:
            act->getaddrinfo.ret = getaddrinfo(act->getaddrinfo.node, act->getaddrinfo.service, act->getaddrinfo.hints, act->getaddrinfo.res);
            act->getaddrinfo.verrno = errno;
            break;
        case IO_GETNAMEINFO:
            act->getnameinfo.ret = getnameinfo(act->getnameinfo.sa, act->getnameinfo.salen, act->getnameinfo.host, act->getnameinfo.hostlen, act->getnameinfo.serv, act->getnameinfo.servlen, act->getnameinfo.flags);
            act->getnameinfo.verrno = errno;
            break;
        case IO_IOCTL:
            act->ioctl.ret = ioctl(act->ioctl.d, act->ioctl.request, act->ioctl.argp);
            act->ioctl.verrno = errno;
            break;
        case IO_GETIFADDRS:
            act->getifaddrs.ret = getifaddrs(act->getifaddrs.ifap);
            act->getifaddrs.verrno = errno;
            break;
        case IO_READV:
            act->readv.ret = readv(act->readv.fd, act->readv.iov, act->readv.iovcnt);
            act->readv.verrno = errno;
            break;
        case IO_WRITEV:
            act->writev.ret = writev(act->writev.fd, act->writev.iov, act->writev.iovcnt);
            act->writev.verrno = errno;
            break;
        case IO_PREADV:
            act->preadv.ret = preadv(act->preadv.fd, act->preadv.iov, act->preadv.iovcnt, act->preadv.offset);
            act->preadv.verrno = errno;
            break;
        case IO_PWRITEV:
            act->pwritev.ret = pwritev(act->pwritev.fd, act->pwritev.iov, act->pwritev.iovcnt, act->pwritev.offset);
            act->pwritev.verrno = errno;
            break;
        case IO_MSYNC:
            act->msync.ret = msync(act->msync.addr, act->msync.len, act->msync.flags);
            act->msync.verrno = errno;
            break;
        case IO_MADVISE:
            act->madvise.ret = madvise(act->madvise.addr, act->madvise.len, act->madvise.advice);
            act->madvise.verrno = errno;
            break;
        case IO_POSIX_MADVISE:
            act->posix_madvise.ret = posix_madvise(act->posix_madvise.addr, act->posix_madvise.len, act->posix_madvise.advice);
            act->posix_madvise.verrno = errno;
            break;
        case IO_MLOCK:
            act->mlock.ret = mlock(act->mlock.addr, act->mlock.len);
            act->mlock.verrno = errno;
            break;
        case IO_MUNLOCK:
            act->munlock.ret = munlock(act->munlock.addr, act->munlock.len);
            act->munlock.verrno = errno;
            break;
        case IO_MLOCKALL:
            act->mlockall.ret = mlockall(act->mlockall.flags);
            act->mlockall.verrno = errno;
            break;
        case IO_MUNLOCKALL:
            act->munlockall.ret = munlockall();
            act->munlockall.verrno = errno;
            break;
        case IO_OPENDIR:
            act->opendir.ret = opendir(act->opendir.name);
            act->opendir.verrno = errno;
            break;
        case IO_FDOPENDIR:
            act->fdopendir.ret = fdopendir(act->fdopendir.fd);
            act->fdopendir.verrno = errno;
            break;
        case IO_CLOSEDIR:
            act->closedir.ret = closedir(act->closedir.dirp);
            act->closedir.verrno = errno;
            break;
        case IO_READDIR_R:
            act->readdir_r.ret = readdir_r(act->readdir_r.dirp, act->readdir_r.entry, act->readdir_r.result);
            act->readdir_r.verrno = errno;
            break;
        case IO_READDIR:
            act->readdir.ret = readdir(act->readdir.dirp);
            act->readdir.verrno = errno;
            break;
        case IO_REWINDDIR:
            rewinddir(act->rewinddir.dirp);
            break;
        case IO_SEEKDIR:
            seekdir(act->seekdir.dirp, act->seekdir.pos);
            break;
        case IO_TELLDIR:
            act->telldir.ret = telldir(act->telldir.dirp);
            act->telldir.verrno = errno;
            break;
        case IO_DIRFD:
            act->dirfd.ret = dirfd(act->dirfd.dirp);
            act->dirfd.verrno = errno;
            break;
        case IO_GLOB:
            act->glob.ret = glob(act->glob.pattern, act->glob.flags, act->glob.errfunc, act->glob.pglob);
            act->glob.verrno = errno;
            break;
        case IO_GLOBFREE:
            globfree(act->globfree.pglob);
            break;
        case IO_POPEN:
            act->popen.ret = popen(act->popen.command, act->popen.type);
            act->popen.verrno = errno;
            break;
        case IO_PCLOSE:
            act->pclose.ret = pclose(act->pclose.stream);
            act->pclose.verrno = errno;
            break;
        case IO_FGETC:
            act->fgetc.ret = fgetc(act->fgetc.stream);
            act->fgetc.verrno = errno;
            break;
        case IO_FGETS:
            act->fgets.ret = fgets(act->fgets.s, act->fgets.size, act->fgets.stream);
            act->fgets.verrno = errno;
            break;
        case IO_KILL:
            act->kill.ret = kill(act->kill.pid, act->kill.sig);
            act->kill.verrno = errno;
            break;
        case IO_FTW:
            act->ftw.ret = ftw(act->ftw.dirpath, act->ftw.cb, act->ftw.nopenfd);
            act->ftw.verrno = errno;
            break;
        case IO_NFTW:
            act->nftw.ret = nftw(act->nftw.dirpath, act->nftw.cb, act->nftw.nopenfd, act->nftw.flags);
            act->nftw.verrno = errno;
            break;
        case IO_WAIT:
            act->wait.ret = wait(act->wait.status);
            act->wait.verrno = errno;
            break;
        case IO_WAITPID:
            act->waitpid.ret = waitpid(act->waitpid.pid, act->waitpid.status, act->waitpid.options);
            act->waitpid.verrno = errno;
            break;
        case IO_WAIT3:
            act->wait3.ret = wait3(act->wait3.stat_loc, act->wait3.options, act->wait3.usage);
            act->wait3.verrno = errno;
            break;
        case IO_WAIT4:
            act->wait4.ret = wait4(act->wait4.pid, act->wait4.stat_loc, act->wait4.options, act->wait4.usage);
            act->wait4.verrno = errno;
            break;
        case IO_DUP:
            act->dup.ret = dup(act->dup.oldfd);
            act->dup.verrno = errno;
            break;
        case IO_DUP2:
            act->dup2.ret = dup2(act->dup2.oldfd, act->dup2.newfd);
            act->dup2.verrno = errno;
            break;
        case IO_ALARM:
            act->alarm.ret = alarm(act->alarm.seconds);
            act->alarm.verrno = errno;
            break;
        case IO_UALARM:
            act->ualarm.ret = ualarm(act->ualarm.value, act->ualarm.interval);
            act->ualarm.verrno = errno;
            break;
        case IO_CHOWN:
            act->chown.ret = chown(act->chown.filename, act->chown.owner, act->chown.group);
            act->chown.verrno = errno;
            break;
        case IO_FCHOWN:
            act->fchown.ret = fchown(act->fchown.fd, act->fchown.owner, act->fchown.group);
            act->fchown.verrno = errno;
            break;
        case IO_LCHOWN:
            act->lchown.ret = lchown(act->lchown.filename, act->lchown.owner, act->lchown.group);
            act->lchown.verrno = errno;
            break;
        case IO_FCHOWNAT:
            act->fchownat.ret = fchownat(act->fchownat.fd, act->fchownat.filename, act->fchownat.owner, act->fchownat.group, act->fchownat.flag);
            act->fchownat.verrno = errno;
            break;
        case IO_FCHDIR:
            act->fchdir.ret = fchdir(act->fchdir.fd);
            act->fchdir.verrno = errno;
            break;
        case IO_GETCWD:
            act->getcwd.ret = getcwd(act->getcwd.buf, act->getcwd.size);
            act->getcwd.verrno = errno;
            break;
        case IO_EXECVE:
            act->execve.ret = execve(act->execve.path, act->execve.argv, act->execve.envp);
            act->execve.verrno = errno;
            break;
        case IO_EXECV:
            act->execv.ret = execv(act->execv.path, act->execv.argv);
            act->execv.verrno = errno;
            break;
        case IO_EXECVPE:
            act->execvpe.ret = execvpe(act->execvpe.file, act->execvpe.argv, act->execvpe.envp);
            act->execvpe.verrno = errno;
            break;
        case IO_GETPID:
            act->getpid.ret = getpid();
            act->getpid.verrno = errno;
            break;
        case IO_GETPPID:
            act->getppid.ret = getppid();
            act->getppid.verrno = errno;
            break;
        case IO_GETPGRP:
            act->getpgrp.ret = getpgrp();
            act->getpgrp.verrno = errno;
            break;
        case IO_GETPGID:
            act->getpgid.ret = getpgid(act->getpgid.pid);
            act->getpgid.verrno = errno;
            break;
        case IO_SETPGID:
            act->setpgid.ret = setpgid(act->setpgid.pid, act->setpgid.pgid);
            act->setpgid.verrno = errno;
            break;
        case IO_SETPGRP:
            act->setpgrp.ret = setpgrp();
            act->setpgrp.verrno = errno;
            break;
        case IO_SETSID:
            act->setsid.ret = setsid();
            act->setsid.verrno = errno;
            break;
        case IO_GETSID:
            act->getsid.ret = getsid(act->getsid.pid);
            act->getsid.verrno = errno;
            break;
        case IO_GETUID:
            act->getuid.ret = getuid();
            act->getuid.verrno = errno;
            break;
        case IO_GETEUID:
            act->geteuid.ret = geteuid();
            act->geteuid.verrno = errno;
            break;
        case IO_GETGID:
            act->getgid.ret = getgid();
            act->getgid.verrno = errno;
            break;
        case IO_FORK:
            act->fork.ret = fork();
            act->fork.verrno = errno;
            break;
        case IO_LINK:
            act->link.ret = link(act->link.from, act->link.to);
            act->link.verrno = errno;
            break;
        case IO_SYMLINK:
            act->symlink.ret = symlink(act->symlink.from, act->symlink.to);
            act->symlink.verrno = errno;
            break;
        case IO_READLINK:
            act->readlink.ret = readlink(act->readlink.path, act->readlink.buf, act->readlink.len);
            act->readlink.verrno = errno;
            break;
        case IO_UNLINK:
            act->unlink.ret = unlink(act->unlink.name);
            act->unlink.verrno = errno;
            break;
        case IO_RMDIR:
            act->rmdir.ret = rmdir(act->rmdir.path);
            act->rmdir.verrno = errno;
            break;
    }
}

int wio_creat(const char* filename, mode_t mode)
{
    struct wire_io_act act;
    act.type = IO_CREAT;
    act.creat.filename = filename;
    act.creat.mode = mode;
    submit_action(&act.common);
    errno = act.creat.verrno;
    return act.creat.ret;
}

int wio_creat64(const char* filename, mode_t mode)
{
    struct wire_io_act act;
    act.type = IO_CREAT64;
    act.creat64.filename = filename;
    act.creat64.mode = mode;
    submit_action(&act.common);
    errno = act.creat64.verrno;
    return act.creat64.ret;
}

int wio_open(const char *pathname, int flags, mode_t mode)
{
    struct wire_io_act act;
    act.type = IO_OPEN;
    act.open.pathname = pathname;
    act.open.flags = flags;
    act.open.mode = mode;
    submit_action(&act.common);
    errno = act.open.verrno;
    return act.open.ret;
}

int wio_close(int fd)
{
    struct wire_io_act act;
    act.type = IO_CLOSE;
    act.close.fd = fd;
    submit_action(&act.common);
    errno = act.close.verrno;
    return act.close.ret;
}

ssize_t wio_pread(int fd, void *buf, size_t count, off_t offset)
{
    struct wire_io_act act;
    act.type = IO_PREAD;
    act.pread.fd = fd;
    act.pread.buf = buf;
    act.pread.count = count;
    act.pread.offset = offset;
    submit_action(&act.common);
    errno = act.pread.verrno;
    return act.pread.ret;
}

ssize_t wio_pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    struct wire_io_act act;
    act.type = IO_PWRITE;
    act.pwrite.fd = fd;
    act.pwrite.buf = buf;
    act.pwrite.count = count;
    act.pwrite.offset = offset;
    submit_action(&act.common);
    errno = act.pwrite.verrno;
    return act.pwrite.ret;
}

ssize_t wio_read(int fd, void *buf, size_t count)
{
    struct wire_io_act act;
    act.type = IO_READ;
    act.read.fd = fd;
    act.read.buf = buf;
    act.read.count = count;
    submit_action(&act.common);
    errno = act.read.verrno;
    return act.read.ret;
}

ssize_t wio_write(int fd, const void *buf, size_t count)
{
    struct wire_io_act act;
    act.type = IO_WRITE;
    act.write.fd = fd;
    act.write.buf = buf;
    act.write.count = count;
    submit_action(&act.common);
    errno = act.write.verrno;
    return act.write.ret;
}

int wio_lockf(int fd, int cmd, off_t len)
{
    struct wire_io_act act;
    act.type = IO_LOCKF;
    act.lockf.fd = fd;
    act.lockf.cmd = cmd;
    act.lockf.len = len;
    submit_action(&act.common);
    errno = act.lockf.verrno;
    return act.lockf.ret;
}

int wio_lockf64(int fd, int cmd, off64_t len)
{
    struct wire_io_act act;
    act.type = IO_LOCKF64;
    act.lockf64.fd = fd;
    act.lockf64.cmd = cmd;
    act.lockf64.len = len;
    submit_action(&act.common);
    errno = act.lockf64.verrno;
    return act.lockf64.ret;
}

int wio_posix_fadvise(int fd, off_t offset, off_t len, int advise)
{
    struct wire_io_act act;
    act.type = IO_POSIX_FADVISE;
    act.posix_fadvise.fd = fd;
    act.posix_fadvise.offset = offset;
    act.posix_fadvise.len = len;
    act.posix_fadvise.advise = advise;
    submit_action(&act.common);
    errno = act.posix_fadvise.verrno;
    return act.posix_fadvise.ret;
}

int wio_posix_fadvise64(int fd, off64_t offset, off64_t len, int advise)
{
    struct wire_io_act act;
    act.type = IO_POSIX_FADVISE64;
    act.posix_fadvise64.fd = fd;
    act.posix_fadvise64.offset = offset;
    act.posix_fadvise64.len = len;
    act.posix_fadvise64.advise = advise;
    submit_action(&act.common);
    errno = act.posix_fadvise64.verrno;
    return act.posix_fadvise64.ret;
}

int wio_posix_fallocate(int fd, off_t offset, off_t len)
{
    struct wire_io_act act;
    act.type = IO_POSIX_FALLOCATE;
    act.posix_fallocate.fd = fd;
    act.posix_fallocate.offset = offset;
    act.posix_fallocate.len = len;
    submit_action(&act.common);
    errno = act.posix_fallocate.verrno;
    return act.posix_fallocate.ret;
}

int wio_posix_fallocate64(int fd, off64_t offset, off64_t len)
{
    struct wire_io_act act;
    act.type = IO_POSIX_FALLOCATE64;
    act.posix_fallocate64.fd = fd;
    act.posix_fallocate64.offset = offset;
    act.posix_fallocate64.len = len;
    submit_action(&act.common);
    errno = act.posix_fallocate64.verrno;
    return act.posix_fallocate64.ret;
}

int wio_fstat(int fd, struct stat *buf)
{
    struct wire_io_act act;
    act.type = IO_FSTAT;
    act.fstat.fd = fd;
    act.fstat.buf = buf;
    submit_action(&act.common);
    errno = act.fstat.verrno;
    return act.fstat.ret;
}

int wio_stat(const char *path, struct stat *buf)
{
    struct wire_io_act act;
    act.type = IO_STAT;
    act.stat.path = path;
    act.stat.buf = buf;
    submit_action(&act.common);
    errno = act.stat.verrno;
    return act.stat.ret;
}

off_t wio_lseek(int fd, off_t offset, int whence)
{
    struct wire_io_act act;
    act.type = IO_LSEEK;
    act.lseek.fd = fd;
    act.lseek.offset = offset;
    act.lseek.whence = whence;
    submit_action(&act.common);
    errno = act.lseek.verrno;
    return act.lseek.ret;
}

off64_t wio_lseek64(int fd, off64_t offset, int whence)
{
    struct wire_io_act act;
    act.type = IO_LSEEK64;
    act.lseek64.fd = fd;
    act.lseek64.offset = offset;
    act.lseek64.whence = whence;
    submit_action(&act.common);
    errno = act.lseek64.verrno;
    return act.lseek64.ret;
}

int wio_ftruncate(int fd, off_t length)
{
    struct wire_io_act act;
    act.type = IO_FTRUNCATE;
    act.ftruncate.fd = fd;
    act.ftruncate.length = length;
    submit_action(&act.common);
    errno = act.ftruncate.verrno;
    return act.ftruncate.ret;
}

int wio_truncate(const char* filename, off_t offset)
{
    struct wire_io_act act;
    act.type = IO_TRUNCATE;
    act.truncate.filename = filename;
    act.truncate.offset = offset;
    submit_action(&act.common);
    errno = act.truncate.verrno;
    return act.truncate.ret;
}

int wio_fallocate(int fd, int mode, off_t offset, off_t len)
{
    struct wire_io_act act;
    act.type = IO_FALLOCATE;
    act.fallocate.fd = fd;
    act.fallocate.mode = mode;
    act.fallocate.offset = offset;
    act.fallocate.len = len;
    submit_action(&act.common);
    errno = act.fallocate.verrno;
    return act.fallocate.ret;
}

int wio_fsync(int fd)
{
    struct wire_io_act act;
    act.type = IO_FSYNC;
    act.fsync.fd = fd;
    submit_action(&act.common);
    errno = act.fsync.verrno;
    return act.fsync.ret;
}

void wio_sync(void)
{
    struct wire_io_act act;
    act.type = IO_SYNC;
    submit_action(&act.common);
}

int wio_fdatasync(int fd)
{
    struct wire_io_act act;
    act.type = IO_FDATASYNC;
    act.fdatasync.fd = fd;
    submit_action(&act.common);
    errno = act.fdatasync.verrno;
    return act.fdatasync.ret;
}

int wio_statfs(const char *path, struct statfs *buf)
{
    struct wire_io_act act;
    act.type = IO_STATFS;
    act.statfs.path = path;
    act.statfs.buf = buf;
    submit_action(&act.common);
    errno = act.statfs.verrno;
    return act.statfs.ret;
}

int wio_fstatfs(int fd, struct statfs *buf)
{
    struct wire_io_act act;
    act.type = IO_FSTATFS;
    act.fstatfs.fd = fd;
    act.fstatfs.buf = buf;
    submit_action(&act.common);
    errno = act.fstatfs.verrno;
    return act.fstatfs.ret;
}

int wio_getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    struct wire_io_act act;
    act.type = IO_GETADDRINFO;
    act.getaddrinfo.node = node;
    act.getaddrinfo.service = service;
    act.getaddrinfo.hints = hints;
    act.getaddrinfo.res = res;
    submit_action(&act.common);
    errno = act.getaddrinfo.verrno;
    return act.getaddrinfo.ret;
}

int wio_getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags)
{
    struct wire_io_act act;
    act.type = IO_GETNAMEINFO;
    act.getnameinfo.sa = sa;
    act.getnameinfo.salen = salen;
    act.getnameinfo.host = host;
    act.getnameinfo.hostlen = hostlen;
    act.getnameinfo.serv = serv;
    act.getnameinfo.servlen = servlen;
    act.getnameinfo.flags = flags;
    submit_action(&act.common);
    errno = act.getnameinfo.verrno;
    return act.getnameinfo.ret;
}

int wio_ioctl(int d, unsigned long request, void *argp)
{
    struct wire_io_act act;
    act.type = IO_IOCTL;
    act.ioctl.d = d;
    act.ioctl.request = request;
    act.ioctl.argp = argp;
    submit_action(&act.common);
    errno = act.ioctl.verrno;
    return act.ioctl.ret;
}

int wio_getifaddrs(struct ifaddrs **ifap)
{
    struct wire_io_act act;
    act.type = IO_GETIFADDRS;
    act.getifaddrs.ifap = ifap;
    submit_action(&act.common);
    errno = act.getifaddrs.verrno;
    return act.getifaddrs.ret;
}

ssize_t wio_readv(int fd, const struct iovec *iov, int iovcnt)
{
    struct wire_io_act act;
    act.type = IO_READV;
    act.readv.fd = fd;
    act.readv.iov = iov;
    act.readv.iovcnt = iovcnt;
    submit_action(&act.common);
    errno = act.readv.verrno;
    return act.readv.ret;
}

ssize_t wio_writev(int fd, const struct iovec *iov, int iovcnt)
{
    struct wire_io_act act;
    act.type = IO_WRITEV;
    act.writev.fd = fd;
    act.writev.iov = iov;
    act.writev.iovcnt = iovcnt;
    submit_action(&act.common);
    errno = act.writev.verrno;
    return act.writev.ret;
}

ssize_t wio_preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    struct wire_io_act act;
    act.type = IO_PREADV;
    act.preadv.fd = fd;
    act.preadv.iov = iov;
    act.preadv.iovcnt = iovcnt;
    act.preadv.offset = offset;
    submit_action(&act.common);
    errno = act.preadv.verrno;
    return act.preadv.ret;
}

ssize_t wio_pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    struct wire_io_act act;
    act.type = IO_PWRITEV;
    act.pwritev.fd = fd;
    act.pwritev.iov = iov;
    act.pwritev.iovcnt = iovcnt;
    act.pwritev.offset = offset;
    submit_action(&act.common);
    errno = act.pwritev.verrno;
    return act.pwritev.ret;
}

int wio_msync(void* addr, size_t len, int flags)
{
    struct wire_io_act act;
    act.type = IO_MSYNC;
    act.msync.addr = addr;
    act.msync.len = len;
    act.msync.flags = flags;
    submit_action(&act.common);
    errno = act.msync.verrno;
    return act.msync.ret;
}

int wio_madvise(void* addr, size_t len, int advice)
{
    struct wire_io_act act;
    act.type = IO_MADVISE;
    act.madvise.addr = addr;
    act.madvise.len = len;
    act.madvise.advice = advice;
    submit_action(&act.common);
    errno = act.madvise.verrno;
    return act.madvise.ret;
}

int wio_posix_madvise(void* addr, size_t len, int advice)
{
    struct wire_io_act act;
    act.type = IO_POSIX_MADVISE;
    act.posix_madvise.addr = addr;
    act.posix_madvise.len = len;
    act.posix_madvise.advice = advice;
    submit_action(&act.common);
    errno = act.posix_madvise.verrno;
    return act.posix_madvise.ret;
}

int wio_mlock(const void* addr, size_t len)
{
    struct wire_io_act act;
    act.type = IO_MLOCK;
    act.mlock.addr = addr;
    act.mlock.len = len;
    submit_action(&act.common);
    errno = act.mlock.verrno;
    return act.mlock.ret;
}

int wio_munlock(const void* addr, size_t len)
{
    struct wire_io_act act;
    act.type = IO_MUNLOCK;
    act.munlock.addr = addr;
    act.munlock.len = len;
    submit_action(&act.common);
    errno = act.munlock.verrno;
    return act.munlock.ret;
}

int wio_mlockall(int flags)
{
    struct wire_io_act act;
    act.type = IO_MLOCKALL;
    act.mlockall.flags = flags;
    submit_action(&act.common);
    errno = act.mlockall.verrno;
    return act.mlockall.ret;
}

int wio_munlockall(void)
{
    struct wire_io_act act;
    act.type = IO_MUNLOCKALL;
    submit_action(&act.common);
    errno = act.munlockall.verrno;
    return act.munlockall.ret;
}

DIR * wio_opendir(const char *name)
{
    struct wire_io_act act;
    act.type = IO_OPENDIR;
    act.opendir.name = name;
    submit_action(&act.common);
    errno = act.opendir.verrno;
    return act.opendir.ret;
}

DIR * wio_fdopendir(int fd)
{
    struct wire_io_act act;
    act.type = IO_FDOPENDIR;
    act.fdopendir.fd = fd;
    submit_action(&act.common);
    errno = act.fdopendir.verrno;
    return act.fdopendir.ret;
}

int wio_closedir(DIR *dirp)
{
    struct wire_io_act act;
    act.type = IO_CLOSEDIR;
    act.closedir.dirp = dirp;
    submit_action(&act.common);
    errno = act.closedir.verrno;
    return act.closedir.ret;
}

int wio_readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    struct wire_io_act act;
    act.type = IO_READDIR_R;
    act.readdir_r.dirp = dirp;
    act.readdir_r.entry = entry;
    act.readdir_r.result = result;
    submit_action(&act.common);
    errno = act.readdir_r.verrno;
    return act.readdir_r.ret;
}

struct dirent * wio_readdir(DIR *dirp)
{
    struct wire_io_act act;
    act.type = IO_READDIR;
    act.readdir.dirp = dirp;
    submit_action(&act.common);
    errno = act.readdir.verrno;
    return act.readdir.ret;
}

void wio_rewinddir(DIR* dirp)
{
    struct wire_io_act act;
    act.type = IO_REWINDDIR;
    act.rewinddir.dirp = dirp;
    submit_action(&act.common);
}

void wio_seekdir(DIR* dirp, long int pos)
{
    struct wire_io_act act;
    act.type = IO_SEEKDIR;
    act.seekdir.dirp = dirp;
    act.seekdir.pos = pos;
    submit_action(&act.common);
}

long int wio_telldir(DIR* dirp)
{
    struct wire_io_act act;
    act.type = IO_TELLDIR;
    act.telldir.dirp = dirp;
    submit_action(&act.common);
    errno = act.telldir.verrno;
    return act.telldir.ret;
}

int wio_dirfd(DIR* dirp)
{
    struct wire_io_act act;
    act.type = IO_DIRFD;
    act.dirfd.dirp = dirp;
    submit_action(&act.common);
    errno = act.dirfd.verrno;
    return act.dirfd.ret;
}

int wio_glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob)
{
    struct wire_io_act act;
    act.type = IO_GLOB;
    act.glob.pattern = pattern;
    act.glob.flags = flags;
    act.glob.errfunc = errfunc;
    act.glob.pglob = pglob;
    submit_action(&act.common);
    errno = act.glob.verrno;
    return act.glob.ret;
}

void wio_globfree(glob_t *pglob)
{
    struct wire_io_act act;
    act.type = IO_GLOBFREE;
    act.globfree.pglob = pglob;
    submit_action(&act.common);
}

FILE * wio_popen(const char *command, const char *type)
{
    struct wire_io_act act;
    act.type = IO_POPEN;
    act.popen.command = command;
    act.popen.type = type;
    submit_action(&act.common);
    errno = act.popen.verrno;
    return act.popen.ret;
}

int wio_pclose(FILE *stream)
{
    struct wire_io_act act;
    act.type = IO_PCLOSE;
    act.pclose.stream = stream;
    submit_action(&act.common);
    errno = act.pclose.verrno;
    return act.pclose.ret;
}

int wio_fgetc(FILE *stream)
{
    struct wire_io_act act;
    act.type = IO_FGETC;
    act.fgetc.stream = stream;
    submit_action(&act.common);
    errno = act.fgetc.verrno;
    return act.fgetc.ret;
}

char * wio_fgets(char *s, int size, FILE *stream)
{
    struct wire_io_act act;
    act.type = IO_FGETS;
    act.fgets.s = s;
    act.fgets.size = size;
    act.fgets.stream = stream;
    submit_action(&act.common);
    errno = act.fgets.verrno;
    return act.fgets.ret;
}

int wio_kill(pid_t pid, int sig)
{
    struct wire_io_act act;
    act.type = IO_KILL;
    act.kill.pid = pid;
    act.kill.sig = sig;
    submit_action(&act.common);
    errno = act.kill.verrno;
    return act.kill.ret;
}

int wio_ftw(const char *dirpath, ftw_cb_t cb, int nopenfd)
{
    struct wire_io_act act;
    act.type = IO_FTW;
    act.ftw.dirpath = dirpath;
    act.ftw.cb = cb;
    act.ftw.nopenfd = nopenfd;
    submit_action(&act.common);
    errno = act.ftw.verrno;
    return act.ftw.ret;
}

int wio_nftw(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags)
{
    struct wire_io_act act;
    act.type = IO_NFTW;
    act.nftw.dirpath = dirpath;
    act.nftw.cb = cb;
    act.nftw.nopenfd = nopenfd;
    act.nftw.flags = flags;
    submit_action(&act.common);
    errno = act.nftw.verrno;
    return act.nftw.ret;
}

pid_t wio_wait(int *status)
{
    struct wire_io_act act;
    act.type = IO_WAIT;
    act.wait.status = status;
    submit_action(&act.common);
    errno = act.wait.verrno;
    return act.wait.ret;
}

pid_t wio_waitpid(pid_t pid, int *status, int options)
{
    struct wire_io_act act;
    act.type = IO_WAITPID;
    act.waitpid.pid = pid;
    act.waitpid.status = status;
    act.waitpid.options = options;
    submit_action(&act.common);
    errno = act.waitpid.verrno;
    return act.waitpid.ret;
}

pid_t wio_wait3(int* stat_loc, int options, struct rusage* usage)
{
    struct wire_io_act act;
    act.type = IO_WAIT3;
    act.wait3.stat_loc = stat_loc;
    act.wait3.options = options;
    act.wait3.usage = usage;
    submit_action(&act.common);
    errno = act.wait3.verrno;
    return act.wait3.ret;
}

pid_t wio_wait4(pid_t pid, int* stat_loc, int options, struct rusage* usage)
{
    struct wire_io_act act;
    act.type = IO_WAIT4;
    act.wait4.pid = pid;
    act.wait4.stat_loc = stat_loc;
    act.wait4.options = options;
    act.wait4.usage = usage;
    submit_action(&act.common);
    errno = act.wait4.verrno;
    return act.wait4.ret;
}

int wio_dup(int oldfd)
{
    struct wire_io_act act;
    act.type = IO_DUP;
    act.dup.oldfd = oldfd;
    submit_action(&act.common);
    errno = act.dup.verrno;
    return act.dup.ret;
}

int wio_dup2(int oldfd, int newfd)
{
    struct wire_io_act act;
    act.type = IO_DUP2;
    act.dup2.oldfd = oldfd;
    act.dup2.newfd = newfd;
    submit_action(&act.common);
    errno = act.dup2.verrno;
    return act.dup2.ret;
}

unsigned int wio_alarm(unsigned int seconds)
{
    struct wire_io_act act;
    act.type = IO_ALARM;
    act.alarm.seconds = seconds;
    submit_action(&act.common);
    errno = act.alarm.verrno;
    return act.alarm.ret;
}

unsigned int wio_ualarm(unsigned value, unsigned interval)
{
    struct wire_io_act act;
    act.type = IO_UALARM;
    act.ualarm.value = value;
    act.ualarm.interval = interval;
    submit_action(&act.common);
    errno = act.ualarm.verrno;
    return act.ualarm.ret;
}

int wio_chown(const char *filename, uid_t owner, gid_t group)
{
    struct wire_io_act act;
    act.type = IO_CHOWN;
    act.chown.filename = filename;
    act.chown.owner = owner;
    act.chown.group = group;
    submit_action(&act.common);
    errno = act.chown.verrno;
    return act.chown.ret;
}

int wio_fchown(int fd, uid_t owner, gid_t group)
{
    struct wire_io_act act;
    act.type = IO_FCHOWN;
    act.fchown.fd = fd;
    act.fchown.owner = owner;
    act.fchown.group = group;
    submit_action(&act.common);
    errno = act.fchown.verrno;
    return act.fchown.ret;
}

int wio_lchown(const char* filename, uid_t owner, gid_t group)
{
    struct wire_io_act act;
    act.type = IO_LCHOWN;
    act.lchown.filename = filename;
    act.lchown.owner = owner;
    act.lchown.group = group;
    submit_action(&act.common);
    errno = act.lchown.verrno;
    return act.lchown.ret;
}

int wio_fchownat(int fd, const char* filename, uid_t owner, gid_t group, int flag)
{
    struct wire_io_act act;
    act.type = IO_FCHOWNAT;
    act.fchownat.fd = fd;
    act.fchownat.filename = filename;
    act.fchownat.owner = owner;
    act.fchownat.group = group;
    act.fchownat.flag = flag;
    submit_action(&act.common);
    errno = act.fchownat.verrno;
    return act.fchownat.ret;
}

int wio_fchdir(int fd)
{
    struct wire_io_act act;
    act.type = IO_FCHDIR;
    act.fchdir.fd = fd;
    submit_action(&act.common);
    errno = act.fchdir.verrno;
    return act.fchdir.ret;
}

char* wio_getcwd(char* buf, size_t size)
{
    struct wire_io_act act;
    act.type = IO_GETCWD;
    act.getcwd.buf = buf;
    act.getcwd.size = size;
    submit_action(&act.common);
    errno = act.getcwd.verrno;
    return act.getcwd.ret;
}

int wio_execve(const char* path, char *const* argv, char *const* envp)
{
    struct wire_io_act act;
    act.type = IO_EXECVE;
    act.execve.path = path;
    act.execve.argv = argv;
    act.execve.envp = envp;
    submit_action(&act.common);
    errno = act.execve.verrno;
    return act.execve.ret;
}

int wio_execv(const char* path, char *const* argv)
{
    struct wire_io_act act;
    act.type = IO_EXECV;
    act.execv.path = path;
    act.execv.argv = argv;
    submit_action(&act.common);
    errno = act.execv.verrno;
    return act.execv.ret;
}

int wio_execvpe(const char* file, char *const* argv, char *const* envp)
{
    struct wire_io_act act;
    act.type = IO_EXECVPE;
    act.execvpe.file = file;
    act.execvpe.argv = argv;
    act.execvpe.envp = envp;
    submit_action(&act.common);
    errno = act.execvpe.verrno;
    return act.execvpe.ret;
}

pid_t wio_getpid(void)
{
    struct wire_io_act act;
    act.type = IO_GETPID;
    submit_action(&act.common);
    errno = act.getpid.verrno;
    return act.getpid.ret;
}

pid_t wio_getppid(void)
{
    struct wire_io_act act;
    act.type = IO_GETPPID;
    submit_action(&act.common);
    errno = act.getppid.verrno;
    return act.getppid.ret;
}

pid_t wio_getpgrp(void)
{
    struct wire_io_act act;
    act.type = IO_GETPGRP;
    submit_action(&act.common);
    errno = act.getpgrp.verrno;
    return act.getpgrp.ret;
}

pid_t wio_getpgid(pid_t pid)
{
    struct wire_io_act act;
    act.type = IO_GETPGID;
    act.getpgid.pid = pid;
    submit_action(&act.common);
    errno = act.getpgid.verrno;
    return act.getpgid.ret;
}

int wio_setpgid(pid_t pid, pid_t pgid)
{
    struct wire_io_act act;
    act.type = IO_SETPGID;
    act.setpgid.pid = pid;
    act.setpgid.pgid = pgid;
    submit_action(&act.common);
    errno = act.setpgid.verrno;
    return act.setpgid.ret;
}

int wio_setpgrp(void)
{
    struct wire_io_act act;
    act.type = IO_SETPGRP;
    submit_action(&act.common);
    errno = act.setpgrp.verrno;
    return act.setpgrp.ret;
}

pid_t wio_setsid(void)
{
    struct wire_io_act act;
    act.type = IO_SETSID;
    submit_action(&act.common);
    errno = act.setsid.verrno;
    return act.setsid.ret;
}

pid_t wio_getsid(pid_t pid)
{
    struct wire_io_act act;
    act.type = IO_GETSID;
    act.getsid.pid = pid;
    submit_action(&act.common);
    errno = act.getsid.verrno;
    return act.getsid.ret;
}

uid_t wio_getuid(void)
{
    struct wire_io_act act;
    act.type = IO_GETUID;
    submit_action(&act.common);
    errno = act.getuid.verrno;
    return act.getuid.ret;
}

uid_t wio_geteuid(void)
{
    struct wire_io_act act;
    act.type = IO_GETEUID;
    submit_action(&act.common);
    errno = act.geteuid.verrno;
    return act.geteuid.ret;
}

gid_t wio_getgid(void)
{
    struct wire_io_act act;
    act.type = IO_GETGID;
    submit_action(&act.common);
    errno = act.getgid.verrno;
    return act.getgid.ret;
}

pid_t wio_fork(void)
{
    struct wire_io_act act;
    act.type = IO_FORK;
    submit_action(&act.common);
    errno = act.fork.verrno;
    return act.fork.ret;
}

int wio_link(const char* from, const char* to)
{
    struct wire_io_act act;
    act.type = IO_LINK;
    act.link.from = from;
    act.link.to = to;
    submit_action(&act.common);
    errno = act.link.verrno;
    return act.link.ret;
}

int wio_symlink(const char* from, const char* to)
{
    struct wire_io_act act;
    act.type = IO_SYMLINK;
    act.symlink.from = from;
    act.symlink.to = to;
    submit_action(&act.common);
    errno = act.symlink.verrno;
    return act.symlink.ret;
}

ssize_t wio_readlink(const char* path, char* buf, size_t len)
{
    struct wire_io_act act;
    act.type = IO_READLINK;
    act.readlink.path = path;
    act.readlink.buf = buf;
    act.readlink.len = len;
    submit_action(&act.common);
    errno = act.readlink.verrno;
    return act.readlink.ret;
}

int wio_unlink(const char *name)
{
    struct wire_io_act act;
    act.type = IO_UNLINK;
    act.unlink.name = name;
    submit_action(&act.common);
    errno = act.unlink.verrno;
    return act.unlink.ret;
}

int wio_rmdir(const char* path)
{
    struct wire_io_act act;
    act.type = IO_RMDIR;
    act.rmdir.path = path;
    submit_action(&act.common);
    errno = act.rmdir.verrno;
    return act.rmdir.ret;
}

static int (*orig_creat)(const char* filename, mode_t mode);
static int (*orig_creat64)(const char* filename, mode_t mode);
static int (*orig_close)(int fd);
static ssize_t (*orig_pread)(int fd, void *buf, size_t count, off_t offset);
static ssize_t (*orig_pwrite)(int fd, const void *buf, size_t count, off_t offset);
static ssize_t (*orig_read)(int fd, void *buf, size_t count);
static ssize_t (*orig_write)(int fd, const void *buf, size_t count);
static int (*orig_lockf)(int fd, int cmd, off_t len);
static int (*orig_lockf64)(int fd, int cmd, off64_t len);
static int (*orig_posix_fadvise)(int fd, off_t offset, off_t len, int advise);
static int (*orig_posix_fadvise64)(int fd, off64_t offset, off64_t len, int advise);
static int (*orig_posix_fallocate)(int fd, off_t offset, off_t len);
static int (*orig_posix_fallocate64)(int fd, off64_t offset, off64_t len);
static off_t (*orig_lseek)(int fd, off_t offset, int whence);
static off64_t (*orig_lseek64)(int fd, off64_t offset, int whence);
static int (*orig_ftruncate)(int fd, off_t length);
static int (*orig_truncate)(const char* filename, off_t offset);
static int (*orig_fallocate)(int fd, int mode, off_t offset, off_t len);
static int (*orig_fsync)(int fd);
static void (*orig_sync)(void);
static int (*orig_fdatasync)(int fd);
static int (*orig_statfs)(const char *path, struct statfs *buf);
static int (*orig_fstatfs)(int fd, struct statfs *buf);
static int (*orig_getaddrinfo)(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res);
static int (*orig_getnameinfo)(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags);
static int (*orig_getifaddrs)(struct ifaddrs **ifap);
static ssize_t (*orig_readv)(int fd, const struct iovec *iov, int iovcnt);
static ssize_t (*orig_writev)(int fd, const struct iovec *iov, int iovcnt);
static ssize_t (*orig_preadv)(int fd, const struct iovec *iov, int iovcnt, off_t offset);
static ssize_t (*orig_pwritev)(int fd, const struct iovec *iov, int iovcnt, off_t offset);
static int (*orig_msync)(void* addr, size_t len, int flags);
static int (*orig_madvise)(void* addr, size_t len, int advice);
static int (*orig_posix_madvise)(void* addr, size_t len, int advice);
static int (*orig_mlock)(const void* addr, size_t len);
static int (*orig_munlock)(const void* addr, size_t len);
static int (*orig_mlockall)(int flags);
static int (*orig_munlockall)(void);
static DIR * (*orig_opendir)(const char *name);
static DIR * (*orig_fdopendir)(int fd);
static int (*orig_closedir)(DIR *dirp);
static int (*orig_readdir_r)(DIR *dirp, struct dirent *entry, struct dirent **result);
static struct dirent * (*orig_readdir)(DIR *dirp);
static void (*orig_rewinddir)(DIR* dirp);
static void (*orig_seekdir)(DIR* dirp, long int pos);
static long int (*orig_telldir)(DIR* dirp);
static int (*orig_dirfd)(DIR* dirp);
static int (*orig_glob)(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob);
static void (*orig_globfree)(glob_t *pglob);
static FILE * (*orig_popen)(const char *command, const char *type);
static int (*orig_pclose)(FILE *stream);
static int (*orig_fgetc)(FILE *stream);
static char * (*orig_fgets)(char *s, int size, FILE *stream);
static int (*orig_kill)(pid_t pid, int sig);
static int (*orig_ftw)(const char *dirpath, ftw_cb_t cb, int nopenfd);
static int (*orig_nftw)(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags);
static pid_t (*orig_wait)(int *status);
static pid_t (*orig_waitpid)(pid_t pid, int *status, int options);
static pid_t (*orig_wait3)(int* stat_loc, int options, struct rusage* usage);
static pid_t (*orig_wait4)(pid_t pid, int* stat_loc, int options, struct rusage* usage);
static int (*orig_dup)(int oldfd);
static int (*orig_dup2)(int oldfd, int newfd);
static unsigned int (*orig_alarm)(unsigned int seconds);
static unsigned int (*orig_ualarm)(unsigned value, unsigned interval);
static int (*orig_chown)(const char *filename, uid_t owner, gid_t group);
static int (*orig_fchown)(int fd, uid_t owner, gid_t group);
static int (*orig_lchown)(const char* filename, uid_t owner, gid_t group);
static int (*orig_fchownat)(int fd, const char* filename, uid_t owner, gid_t group, int flag);
static int (*orig_fchdir)(int fd);
static char* (*orig_getcwd)(char* buf, size_t size);
static int (*orig_execve)(const char* path, char *const* argv, char *const* envp);
static int (*orig_execv)(const char* path, char *const* argv);
static int (*orig_execvpe)(const char* file, char *const* argv, char *const* envp);
static pid_t (*orig_getpid)(void);
static pid_t (*orig_getppid)(void);
static pid_t (*orig_getpgrp)(void);
static pid_t (*orig_getpgid)(pid_t pid);
static int (*orig_setpgid)(pid_t pid, pid_t pgid);
static int (*orig_setpgrp)(void);
static pid_t (*orig_setsid)(void);
static pid_t (*orig_getsid)(pid_t pid);
static uid_t (*orig_getuid)(void);
static uid_t (*orig_geteuid)(void);
static gid_t (*orig_getgid)(void);
static pid_t (*orig_fork)(void);
static int (*orig_link)(const char* from, const char* to);
static int (*orig_symlink)(const char* from, const char* to);
static ssize_t (*orig_readlink)(const char* path, char* buf, size_t len);
static int (*orig_unlink)(const char *name);
static int (*orig_rmdir)(const char* path);
__attribute__((constructor)) static void wire_dlsym_init(void)
{
    orig_creat = dlsym(RTLD_NEXT, "creat");
    if (orig_creat == NULL) { fputs("Failed to get address of creat\n", stderr); abort(); }
    orig_creat64 = dlsym(RTLD_NEXT, "creat64");
    if (orig_creat64 == NULL) { fputs("Failed to get address of creat64\n", stderr); abort(); }
    orig_open = dlsym(RTLD_NEXT, "open");
    if (orig_open == NULL) { fputs("Failed to get address of open\n", stderr); abort(); }
    orig_close = dlsym(RTLD_NEXT, "close");
    if (orig_close == NULL) { fputs("Failed to get address of close\n", stderr); abort(); }
    orig_pread = dlsym(RTLD_NEXT, "pread");
    if (orig_pread == NULL) { fputs("Failed to get address of pread\n", stderr); abort(); }
    orig_pwrite = dlsym(RTLD_NEXT, "pwrite");
    if (orig_pwrite == NULL) { fputs("Failed to get address of pwrite\n", stderr); abort(); }
    orig_read = dlsym(RTLD_NEXT, "read");
    if (orig_read == NULL) { fputs("Failed to get address of read\n", stderr); abort(); }
    orig_write = dlsym(RTLD_NEXT, "write");
    if (orig_write == NULL) { fputs("Failed to get address of write\n", stderr); abort(); }
    orig_lockf = dlsym(RTLD_NEXT, "lockf");
    if (orig_lockf == NULL) { fputs("Failed to get address of lockf\n", stderr); abort(); }
    orig_lockf64 = dlsym(RTLD_NEXT, "lockf64");
    if (orig_lockf64 == NULL) { fputs("Failed to get address of lockf64\n", stderr); abort(); }
    orig_posix_fadvise = dlsym(RTLD_NEXT, "posix_fadvise");
    if (orig_posix_fadvise == NULL) { fputs("Failed to get address of posix_fadvise\n", stderr); abort(); }
    orig_posix_fadvise64 = dlsym(RTLD_NEXT, "posix_fadvise64");
    if (orig_posix_fadvise64 == NULL) { fputs("Failed to get address of posix_fadvise64\n", stderr); abort(); }
    orig_posix_fallocate = dlsym(RTLD_NEXT, "posix_fallocate");
    if (orig_posix_fallocate == NULL) { fputs("Failed to get address of posix_fallocate\n", stderr); abort(); }
    orig_posix_fallocate64 = dlsym(RTLD_NEXT, "posix_fallocate64");
    if (orig_posix_fallocate64 == NULL) { fputs("Failed to get address of posix_fallocate64\n", stderr); abort(); }
    orig_lseek = dlsym(RTLD_NEXT, "lseek");
    if (orig_lseek == NULL) { fputs("Failed to get address of lseek\n", stderr); abort(); }
    orig_lseek64 = dlsym(RTLD_NEXT, "lseek64");
    if (orig_lseek64 == NULL) { fputs("Failed to get address of lseek64\n", stderr); abort(); }
    orig_ftruncate = dlsym(RTLD_NEXT, "ftruncate");
    if (orig_ftruncate == NULL) { fputs("Failed to get address of ftruncate\n", stderr); abort(); }
    orig_truncate = dlsym(RTLD_NEXT, "truncate");
    if (orig_truncate == NULL) { fputs("Failed to get address of truncate\n", stderr); abort(); }
    orig_fallocate = dlsym(RTLD_NEXT, "fallocate");
    if (orig_fallocate == NULL) { fputs("Failed to get address of fallocate\n", stderr); abort(); }
    orig_fsync = dlsym(RTLD_NEXT, "fsync");
    if (orig_fsync == NULL) { fputs("Failed to get address of fsync\n", stderr); abort(); }
    orig_sync = dlsym(RTLD_NEXT, "sync");
    if (orig_sync == NULL) { fputs("Failed to get address of sync\n", stderr); abort(); }
    orig_fdatasync = dlsym(RTLD_NEXT, "fdatasync");
    if (orig_fdatasync == NULL) { fputs("Failed to get address of fdatasync\n", stderr); abort(); }
    orig_statfs = dlsym(RTLD_NEXT, "statfs");
    if (orig_statfs == NULL) { fputs("Failed to get address of statfs\n", stderr); abort(); }
    orig_fstatfs = dlsym(RTLD_NEXT, "fstatfs");
    if (orig_fstatfs == NULL) { fputs("Failed to get address of fstatfs\n", stderr); abort(); }
    orig_getaddrinfo = dlsym(RTLD_NEXT, "getaddrinfo");
    if (orig_getaddrinfo == NULL) { fputs("Failed to get address of getaddrinfo\n", stderr); abort(); }
    orig_getnameinfo = dlsym(RTLD_NEXT, "getnameinfo");
    if (orig_getnameinfo == NULL) { fputs("Failed to get address of getnameinfo\n", stderr); abort(); }
    orig_ioctl = dlsym(RTLD_NEXT, "ioctl");
    if (orig_ioctl == NULL) { fputs("Failed to get address of ioctl\n", stderr); abort(); }
    orig_getifaddrs = dlsym(RTLD_NEXT, "getifaddrs");
    if (orig_getifaddrs == NULL) { fputs("Failed to get address of getifaddrs\n", stderr); abort(); }
    orig_readv = dlsym(RTLD_NEXT, "readv");
    if (orig_readv == NULL) { fputs("Failed to get address of readv\n", stderr); abort(); }
    orig_writev = dlsym(RTLD_NEXT, "writev");
    if (orig_writev == NULL) { fputs("Failed to get address of writev\n", stderr); abort(); }
    orig_preadv = dlsym(RTLD_NEXT, "preadv");
    if (orig_preadv == NULL) { fputs("Failed to get address of preadv\n", stderr); abort(); }
    orig_pwritev = dlsym(RTLD_NEXT, "pwritev");
    if (orig_pwritev == NULL) { fputs("Failed to get address of pwritev\n", stderr); abort(); }
    orig_msync = dlsym(RTLD_NEXT, "msync");
    if (orig_msync == NULL) { fputs("Failed to get address of msync\n", stderr); abort(); }
    orig_madvise = dlsym(RTLD_NEXT, "madvise");
    if (orig_madvise == NULL) { fputs("Failed to get address of madvise\n", stderr); abort(); }
    orig_posix_madvise = dlsym(RTLD_NEXT, "posix_madvise");
    if (orig_posix_madvise == NULL) { fputs("Failed to get address of posix_madvise\n", stderr); abort(); }
    orig_mlock = dlsym(RTLD_NEXT, "mlock");
    if (orig_mlock == NULL) { fputs("Failed to get address of mlock\n", stderr); abort(); }
    orig_munlock = dlsym(RTLD_NEXT, "munlock");
    if (orig_munlock == NULL) { fputs("Failed to get address of munlock\n", stderr); abort(); }
    orig_mlockall = dlsym(RTLD_NEXT, "mlockall");
    if (orig_mlockall == NULL) { fputs("Failed to get address of mlockall\n", stderr); abort(); }
    orig_munlockall = dlsym(RTLD_NEXT, "munlockall");
    if (orig_munlockall == NULL) { fputs("Failed to get address of munlockall\n", stderr); abort(); }
    orig_opendir = dlsym(RTLD_NEXT, "opendir");
    if (orig_opendir == NULL) { fputs("Failed to get address of opendir\n", stderr); abort(); }
    orig_fdopendir = dlsym(RTLD_NEXT, "fdopendir");
    if (orig_fdopendir == NULL) { fputs("Failed to get address of fdopendir\n", stderr); abort(); }
    orig_closedir = dlsym(RTLD_NEXT, "closedir");
    if (orig_closedir == NULL) { fputs("Failed to get address of closedir\n", stderr); abort(); }
    orig_readdir_r = dlsym(RTLD_NEXT, "readdir_r");
    if (orig_readdir_r == NULL) { fputs("Failed to get address of readdir_r\n", stderr); abort(); }
    orig_readdir = dlsym(RTLD_NEXT, "readdir");
    if (orig_readdir == NULL) { fputs("Failed to get address of readdir\n", stderr); abort(); }
    orig_rewinddir = dlsym(RTLD_NEXT, "rewinddir");
    if (orig_rewinddir == NULL) { fputs("Failed to get address of rewinddir\n", stderr); abort(); }
    orig_seekdir = dlsym(RTLD_NEXT, "seekdir");
    if (orig_seekdir == NULL) { fputs("Failed to get address of seekdir\n", stderr); abort(); }
    orig_telldir = dlsym(RTLD_NEXT, "telldir");
    if (orig_telldir == NULL) { fputs("Failed to get address of telldir\n", stderr); abort(); }
    orig_dirfd = dlsym(RTLD_NEXT, "dirfd");
    if (orig_dirfd == NULL) { fputs("Failed to get address of dirfd\n", stderr); abort(); }
    orig_glob = dlsym(RTLD_NEXT, "glob");
    if (orig_glob == NULL) { fputs("Failed to get address of glob\n", stderr); abort(); }
    orig_globfree = dlsym(RTLD_NEXT, "globfree");
    if (orig_globfree == NULL) { fputs("Failed to get address of globfree\n", stderr); abort(); }
    orig_popen = dlsym(RTLD_NEXT, "popen");
    if (orig_popen == NULL) { fputs("Failed to get address of popen\n", stderr); abort(); }
    orig_pclose = dlsym(RTLD_NEXT, "pclose");
    if (orig_pclose == NULL) { fputs("Failed to get address of pclose\n", stderr); abort(); }
    orig_fgetc = dlsym(RTLD_NEXT, "fgetc");
    if (orig_fgetc == NULL) { fputs("Failed to get address of fgetc\n", stderr); abort(); }
    orig_fgets = dlsym(RTLD_NEXT, "fgets");
    if (orig_fgets == NULL) { fputs("Failed to get address of fgets\n", stderr); abort(); }
    orig_kill = dlsym(RTLD_NEXT, "kill");
    if (orig_kill == NULL) { fputs("Failed to get address of kill\n", stderr); abort(); }
    orig_ftw = dlsym(RTLD_NEXT, "ftw");
    if (orig_ftw == NULL) { fputs("Failed to get address of ftw\n", stderr); abort(); }
    orig_nftw = dlsym(RTLD_NEXT, "nftw");
    if (orig_nftw == NULL) { fputs("Failed to get address of nftw\n", stderr); abort(); }
    orig_wait = dlsym(RTLD_NEXT, "wait");
    if (orig_wait == NULL) { fputs("Failed to get address of wait\n", stderr); abort(); }
    orig_waitpid = dlsym(RTLD_NEXT, "waitpid");
    if (orig_waitpid == NULL) { fputs("Failed to get address of waitpid\n", stderr); abort(); }
    orig_wait3 = dlsym(RTLD_NEXT, "wait3");
    if (orig_wait3 == NULL) { fputs("Failed to get address of wait3\n", stderr); abort(); }
    orig_wait4 = dlsym(RTLD_NEXT, "wait4");
    if (orig_wait4 == NULL) { fputs("Failed to get address of wait4\n", stderr); abort(); }
    orig_dup = dlsym(RTLD_NEXT, "dup");
    if (orig_dup == NULL) { fputs("Failed to get address of dup\n", stderr); abort(); }
    orig_dup2 = dlsym(RTLD_NEXT, "dup2");
    if (orig_dup2 == NULL) { fputs("Failed to get address of dup2\n", stderr); abort(); }
    orig_alarm = dlsym(RTLD_NEXT, "alarm");
    if (orig_alarm == NULL) { fputs("Failed to get address of alarm\n", stderr); abort(); }
    orig_ualarm = dlsym(RTLD_NEXT, "ualarm");
    if (orig_ualarm == NULL) { fputs("Failed to get address of ualarm\n", stderr); abort(); }
    orig_chown = dlsym(RTLD_NEXT, "chown");
    if (orig_chown == NULL) { fputs("Failed to get address of chown\n", stderr); abort(); }
    orig_fchown = dlsym(RTLD_NEXT, "fchown");
    if (orig_fchown == NULL) { fputs("Failed to get address of fchown\n", stderr); abort(); }
    orig_lchown = dlsym(RTLD_NEXT, "lchown");
    if (orig_lchown == NULL) { fputs("Failed to get address of lchown\n", stderr); abort(); }
    orig_fchownat = dlsym(RTLD_NEXT, "fchownat");
    if (orig_fchownat == NULL) { fputs("Failed to get address of fchownat\n", stderr); abort(); }
    orig_fchdir = dlsym(RTLD_NEXT, "fchdir");
    if (orig_fchdir == NULL) { fputs("Failed to get address of fchdir\n", stderr); abort(); }
    orig_getcwd = dlsym(RTLD_NEXT, "getcwd");
    if (orig_getcwd == NULL) { fputs("Failed to get address of getcwd\n", stderr); abort(); }
    orig_execve = dlsym(RTLD_NEXT, "execve");
    if (orig_execve == NULL) { fputs("Failed to get address of execve\n", stderr); abort(); }
    orig_execv = dlsym(RTLD_NEXT, "execv");
    if (orig_execv == NULL) { fputs("Failed to get address of execv\n", stderr); abort(); }
    orig_execvpe = dlsym(RTLD_NEXT, "execvpe");
    if (orig_execvpe == NULL) { fputs("Failed to get address of execvpe\n", stderr); abort(); }
    orig_getpid = dlsym(RTLD_NEXT, "getpid");
    if (orig_getpid == NULL) { fputs("Failed to get address of getpid\n", stderr); abort(); }
    orig_getppid = dlsym(RTLD_NEXT, "getppid");
    if (orig_getppid == NULL) { fputs("Failed to get address of getppid\n", stderr); abort(); }
    orig_getpgrp = dlsym(RTLD_NEXT, "getpgrp");
    if (orig_getpgrp == NULL) { fputs("Failed to get address of getpgrp\n", stderr); abort(); }
    orig_getpgid = dlsym(RTLD_NEXT, "getpgid");
    if (orig_getpgid == NULL) { fputs("Failed to get address of getpgid\n", stderr); abort(); }
    orig_setpgid = dlsym(RTLD_NEXT, "setpgid");
    if (orig_setpgid == NULL) { fputs("Failed to get address of setpgid\n", stderr); abort(); }
    orig_setpgrp = dlsym(RTLD_NEXT, "setpgrp");
    if (orig_setpgrp == NULL) { fputs("Failed to get address of setpgrp\n", stderr); abort(); }
    orig_setsid = dlsym(RTLD_NEXT, "setsid");
    if (orig_setsid == NULL) { fputs("Failed to get address of setsid\n", stderr); abort(); }
    orig_getsid = dlsym(RTLD_NEXT, "getsid");
    if (orig_getsid == NULL) { fputs("Failed to get address of getsid\n", stderr); abort(); }
    orig_getuid = dlsym(RTLD_NEXT, "getuid");
    if (orig_getuid == NULL) { fputs("Failed to get address of getuid\n", stderr); abort(); }
    orig_geteuid = dlsym(RTLD_NEXT, "geteuid");
    if (orig_geteuid == NULL) { fputs("Failed to get address of geteuid\n", stderr); abort(); }
    orig_getgid = dlsym(RTLD_NEXT, "getgid");
    if (orig_getgid == NULL) { fputs("Failed to get address of getgid\n", stderr); abort(); }
    orig_fork = dlsym(RTLD_NEXT, "fork");
    if (orig_fork == NULL) { fputs("Failed to get address of fork\n", stderr); abort(); }
    orig_link = dlsym(RTLD_NEXT, "link");
    if (orig_link == NULL) { fputs("Failed to get address of link\n", stderr); abort(); }
    orig_symlink = dlsym(RTLD_NEXT, "symlink");
    if (orig_symlink == NULL) { fputs("Failed to get address of symlink\n", stderr); abort(); }
    orig_readlink = dlsym(RTLD_NEXT, "readlink");
    if (orig_readlink == NULL) { fputs("Failed to get address of readlink\n", stderr); abort(); }
    orig_unlink = dlsym(RTLD_NEXT, "unlink");
    if (orig_unlink == NULL) { fputs("Failed to get address of unlink\n", stderr); abort(); }
    orig_rmdir = dlsym(RTLD_NEXT, "rmdir");
    if (orig_rmdir == NULL) { fputs("Failed to get address of rmdir\n", stderr); abort(); }
}
int creat(const char* filename, mode_t mode)
{
    if (is_wire_thread) {
        return wio_creat(filename, mode);
    } else {
        return orig_creat(filename, mode);
    }
}
int creat64(const char* filename, mode_t mode)
{
    if (is_wire_thread) {
        return wio_creat64(filename, mode);
    } else {
        return orig_creat64(filename, mode);
    }
}
static int gen_open(const char *pathname, int flags, mode_t mode)
{
    if (is_wire_thread) {
        return wio_open(pathname, flags, mode);
    } else {
        return orig_open(pathname, flags, mode);
    }
}
int close(int fd)
{
    if (is_wire_thread) {
        return wio_close(fd);
    } else {
        return orig_close(fd);
    }
}
ssize_t pread(int fd, void *buf, size_t count, off_t offset)
{
    if (is_wire_thread) {
        return wio_pread(fd, buf, count, offset);
    } else {
        return orig_pread(fd, buf, count, offset);
    }
}
ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)
{
    if (is_wire_thread) {
        return wio_pwrite(fd, buf, count, offset);
    } else {
        return orig_pwrite(fd, buf, count, offset);
    }
}
ssize_t read(int fd, void *buf, size_t count)
{
    if (is_wire_thread) {
        return wio_read(fd, buf, count);
    } else {
        return orig_read(fd, buf, count);
    }
}
ssize_t write(int fd, const void *buf, size_t count)
{
    if (is_wire_thread) {
        return wio_write(fd, buf, count);
    } else {
        return orig_write(fd, buf, count);
    }
}
int lockf(int fd, int cmd, off_t len)
{
    if (is_wire_thread) {
        return wio_lockf(fd, cmd, len);
    } else {
        return orig_lockf(fd, cmd, len);
    }
}
int lockf64(int fd, int cmd, off64_t len)
{
    if (is_wire_thread) {
        return wio_lockf64(fd, cmd, len);
    } else {
        return orig_lockf64(fd, cmd, len);
    }
}
int posix_fadvise(int fd, off_t offset, off_t len, int advise)
{
    if (is_wire_thread) {
        return wio_posix_fadvise(fd, offset, len, advise);
    } else {
        return orig_posix_fadvise(fd, offset, len, advise);
    }
}
int posix_fadvise64(int fd, off64_t offset, off64_t len, int advise)
{
    if (is_wire_thread) {
        return wio_posix_fadvise64(fd, offset, len, advise);
    } else {
        return orig_posix_fadvise64(fd, offset, len, advise);
    }
}
int posix_fallocate(int fd, off_t offset, off_t len)
{
    if (is_wire_thread) {
        return wio_posix_fallocate(fd, offset, len);
    } else {
        return orig_posix_fallocate(fd, offset, len);
    }
}
int posix_fallocate64(int fd, off64_t offset, off64_t len)
{
    if (is_wire_thread) {
        return wio_posix_fallocate64(fd, offset, len);
    } else {
        return orig_posix_fallocate64(fd, offset, len);
    }
}
off_t lseek(int fd, off_t offset, int whence)
{
    if (is_wire_thread) {
        return wio_lseek(fd, offset, whence);
    } else {
        return orig_lseek(fd, offset, whence);
    }
}
off64_t lseek64(int fd, off64_t offset, int whence)
{
    if (is_wire_thread) {
        return wio_lseek64(fd, offset, whence);
    } else {
        return orig_lseek64(fd, offset, whence);
    }
}
int ftruncate(int fd, off_t length)
{
    if (is_wire_thread) {
        return wio_ftruncate(fd, length);
    } else {
        return orig_ftruncate(fd, length);
    }
}
int truncate(const char* filename, off_t offset)
{
    if (is_wire_thread) {
        return wio_truncate(filename, offset);
    } else {
        return orig_truncate(filename, offset);
    }
}
int fallocate(int fd, int mode, off_t offset, off_t len)
{
    if (is_wire_thread) {
        return wio_fallocate(fd, mode, offset, len);
    } else {
        return orig_fallocate(fd, mode, offset, len);
    }
}
int fsync(int fd)
{
    if (is_wire_thread) {
        return wio_fsync(fd);
    } else {
        return orig_fsync(fd);
    }
}
void sync(void)
{
    if (is_wire_thread) {
        return wio_sync();
    } else {
        return orig_sync();
    }
}
int fdatasync(int fd)
{
    if (is_wire_thread) {
        return wio_fdatasync(fd);
    } else {
        return orig_fdatasync(fd);
    }
}
int statfs(const char *path, struct statfs *buf)
{
    if (is_wire_thread) {
        return wio_statfs(path, buf);
    } else {
        return orig_statfs(path, buf);
    }
}
int fstatfs(int fd, struct statfs *buf)
{
    if (is_wire_thread) {
        return wio_fstatfs(fd, buf);
    } else {
        return orig_fstatfs(fd, buf);
    }
}
int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)
{
    if (is_wire_thread) {
        return wio_getaddrinfo(node, service, hints, res);
    } else {
        return orig_getaddrinfo(node, service, hints, res);
    }
}
int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags)
{
    if (is_wire_thread) {
        return wio_getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
    } else {
        return orig_getnameinfo(sa, salen, host, hostlen, serv, servlen, flags);
    }
}
static int gen_ioctl(int d, unsigned long request, void *argp)
{
    if (is_wire_thread) {
        return wio_ioctl(d, request, argp);
    } else {
        return orig_ioctl(d, request, argp);
    }
}
int getifaddrs(struct ifaddrs **ifap)
{
    if (is_wire_thread) {
        return wio_getifaddrs(ifap);
    } else {
        return orig_getifaddrs(ifap);
    }
}
ssize_t readv(int fd, const struct iovec *iov, int iovcnt)
{
    if (is_wire_thread) {
        return wio_readv(fd, iov, iovcnt);
    } else {
        return orig_readv(fd, iov, iovcnt);
    }
}
ssize_t writev(int fd, const struct iovec *iov, int iovcnt)
{
    if (is_wire_thread) {
        return wio_writev(fd, iov, iovcnt);
    } else {
        return orig_writev(fd, iov, iovcnt);
    }
}
ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    if (is_wire_thread) {
        return wio_preadv(fd, iov, iovcnt, offset);
    } else {
        return orig_preadv(fd, iov, iovcnt, offset);
    }
}
ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)
{
    if (is_wire_thread) {
        return wio_pwritev(fd, iov, iovcnt, offset);
    } else {
        return orig_pwritev(fd, iov, iovcnt, offset);
    }
}
int msync(void* addr, size_t len, int flags)
{
    if (is_wire_thread) {
        return wio_msync(addr, len, flags);
    } else {
        return orig_msync(addr, len, flags);
    }
}
int madvise(void* addr, size_t len, int advice)
{
    if (is_wire_thread) {
        return wio_madvise(addr, len, advice);
    } else {
        return orig_madvise(addr, len, advice);
    }
}
int posix_madvise(void* addr, size_t len, int advice)
{
    if (is_wire_thread) {
        return wio_posix_madvise(addr, len, advice);
    } else {
        return orig_posix_madvise(addr, len, advice);
    }
}
int mlock(const void* addr, size_t len)
{
    if (is_wire_thread) {
        return wio_mlock(addr, len);
    } else {
        return orig_mlock(addr, len);
    }
}
int munlock(const void* addr, size_t len)
{
    if (is_wire_thread) {
        return wio_munlock(addr, len);
    } else {
        return orig_munlock(addr, len);
    }
}
int mlockall(int flags)
{
    if (is_wire_thread) {
        return wio_mlockall(flags);
    } else {
        return orig_mlockall(flags);
    }
}
int munlockall(void)
{
    if (is_wire_thread) {
        return wio_munlockall();
    } else {
        return orig_munlockall();
    }
}
DIR * opendir(const char *name)
{
    if (is_wire_thread) {
        return wio_opendir(name);
    } else {
        return orig_opendir(name);
    }
}
DIR * fdopendir(int fd)
{
    if (is_wire_thread) {
        return wio_fdopendir(fd);
    } else {
        return orig_fdopendir(fd);
    }
}
int closedir(DIR *dirp)
{
    if (is_wire_thread) {
        return wio_closedir(dirp);
    } else {
        return orig_closedir(dirp);
    }
}
int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)
{
    if (is_wire_thread) {
        return wio_readdir_r(dirp, entry, result);
    } else {
        return orig_readdir_r(dirp, entry, result);
    }
}
struct dirent * readdir(DIR *dirp)
{
    if (is_wire_thread) {
        return wio_readdir(dirp);
    } else {
        return orig_readdir(dirp);
    }
}
void rewinddir(DIR* dirp)
{
    if (is_wire_thread) {
        return wio_rewinddir(dirp);
    } else {
        return orig_rewinddir(dirp);
    }
}
void seekdir(DIR* dirp, long int pos)
{
    if (is_wire_thread) {
        return wio_seekdir(dirp, pos);
    } else {
        return orig_seekdir(dirp, pos);
    }
}
long int telldir(DIR* dirp)
{
    if (is_wire_thread) {
        return wio_telldir(dirp);
    } else {
        return orig_telldir(dirp);
    }
}
int dirfd(DIR* dirp)
{
    if (is_wire_thread) {
        return wio_dirfd(dirp);
    } else {
        return orig_dirfd(dirp);
    }
}
int glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob)
{
    if (is_wire_thread) {
        return wio_glob(pattern, flags, errfunc, pglob);
    } else {
        return orig_glob(pattern, flags, errfunc, pglob);
    }
}
void globfree(glob_t *pglob)
{
    if (is_wire_thread) {
        return wio_globfree(pglob);
    } else {
        return orig_globfree(pglob);
    }
}
FILE * popen(const char *command, const char *type)
{
    if (is_wire_thread) {
        return wio_popen(command, type);
    } else {
        return orig_popen(command, type);
    }
}
int pclose(FILE *stream)
{
    if (is_wire_thread) {
        return wio_pclose(stream);
    } else {
        return orig_pclose(stream);
    }
}
int fgetc(FILE *stream)
{
    if (is_wire_thread) {
        return wio_fgetc(stream);
    } else {
        return orig_fgetc(stream);
    }
}
char * fgets(char *s, int size, FILE *stream)
{
    if (is_wire_thread) {
        return wio_fgets(s, size, stream);
    } else {
        return orig_fgets(s, size, stream);
    }
}
int kill(pid_t pid, int sig)
{
    if (is_wire_thread) {
        return wio_kill(pid, sig);
    } else {
        return orig_kill(pid, sig);
    }
}
int ftw(const char *dirpath, ftw_cb_t cb, int nopenfd)
{
    if (is_wire_thread) {
        return wio_ftw(dirpath, cb, nopenfd);
    } else {
        return orig_ftw(dirpath, cb, nopenfd);
    }
}
int nftw(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags)
{
    if (is_wire_thread) {
        return wio_nftw(dirpath, cb, nopenfd, flags);
    } else {
        return orig_nftw(dirpath, cb, nopenfd, flags);
    }
}
pid_t wait(int *status)
{
    if (is_wire_thread) {
        return wio_wait(status);
    } else {
        return orig_wait(status);
    }
}
pid_t waitpid(pid_t pid, int *status, int options)
{
    if (is_wire_thread) {
        return wio_waitpid(pid, status, options);
    } else {
        return orig_waitpid(pid, status, options);
    }
}
pid_t wait3(int* stat_loc, int options, struct rusage* usage)
{
    if (is_wire_thread) {
        return wio_wait3(stat_loc, options, usage);
    } else {
        return orig_wait3(stat_loc, options, usage);
    }
}
pid_t wait4(pid_t pid, int* stat_loc, int options, struct rusage* usage)
{
    if (is_wire_thread) {
        return wio_wait4(pid, stat_loc, options, usage);
    } else {
        return orig_wait4(pid, stat_loc, options, usage);
    }
}
int dup(int oldfd)
{
    if (is_wire_thread) {
        return wio_dup(oldfd);
    } else {
        return orig_dup(oldfd);
    }
}
int dup2(int oldfd, int newfd)
{
    if (is_wire_thread) {
        return wio_dup2(oldfd, newfd);
    } else {
        return orig_dup2(oldfd, newfd);
    }
}
unsigned int alarm(unsigned int seconds)
{
    if (is_wire_thread) {
        return wio_alarm(seconds);
    } else {
        return orig_alarm(seconds);
    }
}
unsigned int ualarm(unsigned value, unsigned interval)
{
    if (is_wire_thread) {
        return wio_ualarm(value, interval);
    } else {
        return orig_ualarm(value, interval);
    }
}
int chown(const char *filename, uid_t owner, gid_t group)
{
    if (is_wire_thread) {
        return wio_chown(filename, owner, group);
    } else {
        return orig_chown(filename, owner, group);
    }
}
int fchown(int fd, uid_t owner, gid_t group)
{
    if (is_wire_thread) {
        return wio_fchown(fd, owner, group);
    } else {
        return orig_fchown(fd, owner, group);
    }
}
int lchown(const char* filename, uid_t owner, gid_t group)
{
    if (is_wire_thread) {
        return wio_lchown(filename, owner, group);
    } else {
        return orig_lchown(filename, owner, group);
    }
}
int fchownat(int fd, const char* filename, uid_t owner, gid_t group, int flag)
{
    if (is_wire_thread) {
        return wio_fchownat(fd, filename, owner, group, flag);
    } else {
        return orig_fchownat(fd, filename, owner, group, flag);
    }
}
int fchdir(int fd)
{
    if (is_wire_thread) {
        return wio_fchdir(fd);
    } else {
        return orig_fchdir(fd);
    }
}
char* getcwd(char* buf, size_t size)
{
    if (is_wire_thread) {
        return wio_getcwd(buf, size);
    } else {
        return orig_getcwd(buf, size);
    }
}
int execve(const char* path, char *const* argv, char *const* envp)
{
    if (is_wire_thread) {
        return wio_execve(path, argv, envp);
    } else {
        return orig_execve(path, argv, envp);
    }
}
int execv(const char* path, char *const* argv)
{
    if (is_wire_thread) {
        return wio_execv(path, argv);
    } else {
        return orig_execv(path, argv);
    }
}
int execvpe(const char* file, char *const* argv, char *const* envp)
{
    if (is_wire_thread) {
        return wio_execvpe(file, argv, envp);
    } else {
        return orig_execvpe(file, argv, envp);
    }
}
pid_t getpid(void)
{
    if (is_wire_thread) {
        return wio_getpid();
    } else {
        return orig_getpid();
    }
}
pid_t getppid(void)
{
    if (is_wire_thread) {
        return wio_getppid();
    } else {
        return orig_getppid();
    }
}
pid_t getpgrp(void)
{
    if (is_wire_thread) {
        return wio_getpgrp();
    } else {
        return orig_getpgrp();
    }
}
pid_t getpgid(pid_t pid)
{
    if (is_wire_thread) {
        return wio_getpgid(pid);
    } else {
        return orig_getpgid(pid);
    }
}
int setpgid(pid_t pid, pid_t pgid)
{
    if (is_wire_thread) {
        return wio_setpgid(pid, pgid);
    } else {
        return orig_setpgid(pid, pgid);
    }
}
int setpgrp(void)
{
    if (is_wire_thread) {
        return wio_setpgrp();
    } else {
        return orig_setpgrp();
    }
}
pid_t setsid(void)
{
    if (is_wire_thread) {
        return wio_setsid();
    } else {
        return orig_setsid();
    }
}
pid_t getsid(pid_t pid)
{
    if (is_wire_thread) {
        return wio_getsid(pid);
    } else {
        return orig_getsid(pid);
    }
}
uid_t getuid(void)
{
    if (is_wire_thread) {
        return wio_getuid();
    } else {
        return orig_getuid();
    }
}
uid_t geteuid(void)
{
    if (is_wire_thread) {
        return wio_geteuid();
    } else {
        return orig_geteuid();
    }
}
gid_t getgid(void)
{
    if (is_wire_thread) {
        return wio_getgid();
    } else {
        return orig_getgid();
    }
}
pid_t fork(void)
{
    if (is_wire_thread) {
        return wio_fork();
    } else {
        return orig_fork();
    }
}
int link(const char* from, const char* to)
{
    if (is_wire_thread) {
        return wio_link(from, to);
    } else {
        return orig_link(from, to);
    }
}
int symlink(const char* from, const char* to)
{
    if (is_wire_thread) {
        return wio_symlink(from, to);
    } else {
        return orig_symlink(from, to);
    }
}
ssize_t readlink(const char* path, char* buf, size_t len)
{
    if (is_wire_thread) {
        return wio_readlink(path, buf, len);
    } else {
        return orig_readlink(path, buf, len);
    }
}
int unlink(const char *name)
{
    if (is_wire_thread) {
        return wio_unlink(name);
    } else {
        return orig_unlink(name);
    }
}
int rmdir(const char* path)
{
    if (is_wire_thread) {
        return wio_rmdir(path);
    } else {
        return orig_rmdir(path);
    }
}
