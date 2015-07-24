#include "wire_io_gen.h"

enum wio_type {
	IO_OPEN,
	IO_CLOSE,
	IO_PREAD,
	IO_PWRITE,
	IO_READ,
	IO_WRITE,
	IO_FSTAT,
	IO_STAT,
	IO_FTRUNCATE,
	IO_FALLOCATE,
	IO_FSYNC,
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
	IO_MMAP,
	IO_MUNMAP,
	IO_OPENDIR,
	IO_FDOPENDIR,
	IO_CLOSEDIR,
	IO_READDIR_R,
	IO_READDIR,
	IO_READ_FILE_CONTENT,
	IO_GLOB,
	IO_GLOBFREE,
	IO_POPEN,
	IO_PCLOSE,
	IO_FGETC,
	IO_FGETS,
	IO_SPAWN,
	IO_KILL,
	IO_FTW,
	IO_NFTW,
	IO_WAIT,
	IO_WAITPID,
	IO_DUP,
	IO_DUP2,
};

struct wire_io_act {
    struct wire_io_act_common common;
    enum wio_type type;
    union {
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
            off_t length;
            int ret;
            int verrno;
        } ftruncate;
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
            size_t hostlen;
            char * serv;
            size_t servlen;
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
            void * addr;
            size_t length;
            int protc;
            int flags;
            int fd;
            off_t offset;
            void * ret;
            int verrno;
        } mmap;
        struct {
            void * addr;
            size_t length;
            int ret;
            int verrno;
        } munmap;
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
            const char * filename;
            char * buf;
            size_t bufsz;
            int ret;
            int verrno;
        } read_file_content;
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
            char ** args;
            int * stdin_fd;
            int * stdout_fd;
            int * stderr_fd;
            int ret;
            int verrno;
        } spawn;
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
    };
};

static void perform_action(struct wire_io_act *act)
{
    switch (act->type) {
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
        case IO_FSTAT:
            act->fstat.ret = fstat(act->fstat.fd, act->fstat.buf);
            act->fstat.verrno = errno;
            break;
        case IO_STAT:
            act->stat.ret = stat(act->stat.path, act->stat.buf);
            act->stat.verrno = errno;
            break;
        case IO_FTRUNCATE:
            act->ftruncate.ret = ftruncate(act->ftruncate.fd, act->ftruncate.length);
            act->ftruncate.verrno = errno;
            break;
        case IO_FALLOCATE:
            act->fallocate.ret = fallocate(act->fallocate.fd, act->fallocate.mode, act->fallocate.offset, act->fallocate.len);
            act->fallocate.verrno = errno;
            break;
        case IO_FSYNC:
            act->fsync.ret = fsync(act->fsync.fd);
            act->fsync.verrno = errno;
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
        case IO_MMAP:
            act->mmap.ret = mmap(act->mmap.addr, act->mmap.length, act->mmap.protc, act->mmap.flags, act->mmap.fd, act->mmap.offset);
            act->mmap.verrno = errno;
            break;
        case IO_MUNMAP:
            act->munmap.ret = munmap(act->munmap.addr, act->munmap.length);
            act->munmap.verrno = errno;
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
        case IO_READ_FILE_CONTENT:
            act->read_file_content.ret = read_file_content(act->read_file_content.filename, act->read_file_content.buf, act->read_file_content.bufsz);
            act->read_file_content.verrno = errno;
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
        case IO_SPAWN:
            act->spawn.ret = spawn(act->spawn.args, act->spawn.stdin_fd, act->spawn.stdout_fd, act->spawn.stderr_fd);
            act->spawn.verrno = errno;
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
        case IO_DUP:
            act->dup.ret = dup(act->dup.oldfd);
            act->dup.verrno = errno;
            break;
        case IO_DUP2:
            act->dup2.ret = dup2(act->dup2.oldfd, act->dup2.newfd);
            act->dup2.verrno = errno;
            break;
    }
}

int wio_open(const char *pathname, int flags, mode_t mode)
{
    struct wire_io_act act;
    act.type = IO_OPEN;
    act.open.pathname = pathname;
    act.open.flags = flags;
    act.open.mode = mode;
    submit_action(&wire_io, &act.common);
    errno = act.open.verrno;
    return act.open.ret;
}

int wio_close(int fd)
{
    struct wire_io_act act;
    act.type = IO_CLOSE;
    act.close.fd = fd;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.write.verrno;
    return act.write.ret;
}

int wio_fstat(int fd, struct stat *buf)
{
    struct wire_io_act act;
    act.type = IO_FSTAT;
    act.fstat.fd = fd;
    act.fstat.buf = buf;
    submit_action(&wire_io, &act.common);
    errno = act.fstat.verrno;
    return act.fstat.ret;
}

int wio_stat(const char *path, struct stat *buf)
{
    struct wire_io_act act;
    act.type = IO_STAT;
    act.stat.path = path;
    act.stat.buf = buf;
    submit_action(&wire_io, &act.common);
    errno = act.stat.verrno;
    return act.stat.ret;
}

int wio_ftruncate(int fd, off_t length)
{
    struct wire_io_act act;
    act.type = IO_FTRUNCATE;
    act.ftruncate.fd = fd;
    act.ftruncate.length = length;
    submit_action(&wire_io, &act.common);
    errno = act.ftruncate.verrno;
    return act.ftruncate.ret;
}

int wio_fallocate(int fd, int mode, off_t offset, off_t len)
{
    struct wire_io_act act;
    act.type = IO_FALLOCATE;
    act.fallocate.fd = fd;
    act.fallocate.mode = mode;
    act.fallocate.offset = offset;
    act.fallocate.len = len;
    submit_action(&wire_io, &act.common);
    errno = act.fallocate.verrno;
    return act.fallocate.ret;
}

int wio_fsync(int fd)
{
    struct wire_io_act act;
    act.type = IO_FSYNC;
    act.fsync.fd = fd;
    submit_action(&wire_io, &act.common);
    errno = act.fsync.verrno;
    return act.fsync.ret;
}

int wio_statfs(const char *path, struct statfs *buf)
{
    struct wire_io_act act;
    act.type = IO_STATFS;
    act.statfs.path = path;
    act.statfs.buf = buf;
    submit_action(&wire_io, &act.common);
    errno = act.statfs.verrno;
    return act.statfs.ret;
}

int wio_fstatfs(int fd, struct statfs *buf)
{
    struct wire_io_act act;
    act.type = IO_FSTATFS;
    act.fstatfs.fd = fd;
    act.fstatfs.buf = buf;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.getaddrinfo.verrno;
    return act.getaddrinfo.ret;
}

int wio_getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.ioctl.verrno;
    return act.ioctl.ret;
}

int wio_getifaddrs(struct ifaddrs **ifap)
{
    struct wire_io_act act;
    act.type = IO_GETIFADDRS;
    act.getifaddrs.ifap = ifap;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.pwritev.verrno;
    return act.pwritev.ret;
}

void * wio_mmap(void *addr, size_t length, int protc, int flags, int fd, off_t offset)
{
    struct wire_io_act act;
    act.type = IO_MMAP;
    act.mmap.addr = addr;
    act.mmap.length = length;
    act.mmap.protc = protc;
    act.mmap.flags = flags;
    act.mmap.fd = fd;
    act.mmap.offset = offset;
    submit_action(&wire_io, &act.common);
    errno = act.mmap.verrno;
    return act.mmap.ret;
}

int wio_munmap(void *addr, size_t length)
{
    struct wire_io_act act;
    act.type = IO_MUNMAP;
    act.munmap.addr = addr;
    act.munmap.length = length;
    submit_action(&wire_io, &act.common);
    errno = act.munmap.verrno;
    return act.munmap.ret;
}

DIR * wio_opendir(const char *name)
{
    struct wire_io_act act;
    act.type = IO_OPENDIR;
    act.opendir.name = name;
    submit_action(&wire_io, &act.common);
    errno = act.opendir.verrno;
    return act.opendir.ret;
}

DIR * wio_fdopendir(int fd)
{
    struct wire_io_act act;
    act.type = IO_FDOPENDIR;
    act.fdopendir.fd = fd;
    submit_action(&wire_io, &act.common);
    errno = act.fdopendir.verrno;
    return act.fdopendir.ret;
}

int wio_closedir(DIR *dirp)
{
    struct wire_io_act act;
    act.type = IO_CLOSEDIR;
    act.closedir.dirp = dirp;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.readdir_r.verrno;
    return act.readdir_r.ret;
}

struct dirent * wio_readdir(DIR *dirp)
{
    struct wire_io_act act;
    act.type = IO_READDIR;
    act.readdir.dirp = dirp;
    submit_action(&wire_io, &act.common);
    errno = act.readdir.verrno;
    return act.readdir.ret;
}

int wio_read_file_content(const char *filename, char *buf, size_t bufsz)
{
    struct wire_io_act act;
    act.type = IO_READ_FILE_CONTENT;
    act.read_file_content.filename = filename;
    act.read_file_content.buf = buf;
    act.read_file_content.bufsz = bufsz;
    submit_action(&wire_io, &act.common);
    errno = act.read_file_content.verrno;
    return act.read_file_content.ret;
}

int wio_glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob)
{
    struct wire_io_act act;
    act.type = IO_GLOB;
    act.glob.pattern = pattern;
    act.glob.flags = flags;
    act.glob.errfunc = errfunc;
    act.glob.pglob = pglob;
    submit_action(&wire_io, &act.common);
    errno = act.glob.verrno;
    return act.glob.ret;
}

void wio_globfree(glob_t *pglob)
{
    struct wire_io_act act;
    act.type = IO_GLOBFREE;
    act.globfree.pglob = pglob;
    submit_action(&wire_io, &act.common);
}

FILE * wio_popen(const char *command, const char *type)
{
    struct wire_io_act act;
    act.type = IO_POPEN;
    act.popen.command = command;
    act.popen.type = type;
    submit_action(&wire_io, &act.common);
    errno = act.popen.verrno;
    return act.popen.ret;
}

int wio_pclose(FILE *stream)
{
    struct wire_io_act act;
    act.type = IO_PCLOSE;
    act.pclose.stream = stream;
    submit_action(&wire_io, &act.common);
    errno = act.pclose.verrno;
    return act.pclose.ret;
}

int wio_fgetc(FILE *stream)
{
    struct wire_io_act act;
    act.type = IO_FGETC;
    act.fgetc.stream = stream;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.fgets.verrno;
    return act.fgets.ret;
}

int wio_spawn(char **args, int *stdin_fd, int *stdout_fd, int *stderr_fd)
{
    struct wire_io_act act;
    act.type = IO_SPAWN;
    act.spawn.args = args;
    act.spawn.stdin_fd = stdin_fd;
    act.spawn.stdout_fd = stdout_fd;
    act.spawn.stderr_fd = stderr_fd;
    submit_action(&wire_io, &act.common);
    errno = act.spawn.verrno;
    return act.spawn.ret;
}

int wio_kill(pid_t pid, int sig)
{
    struct wire_io_act act;
    act.type = IO_KILL;
    act.kill.pid = pid;
    act.kill.sig = sig;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.nftw.verrno;
    return act.nftw.ret;
}

pid_t wio_wait(int *status)
{
    struct wire_io_act act;
    act.type = IO_WAIT;
    act.wait.status = status;
    submit_action(&wire_io, &act.common);
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
    submit_action(&wire_io, &act.common);
    errno = act.waitpid.verrno;
    return act.waitpid.ret;
}

int wio_dup(int oldfd)
{
    struct wire_io_act act;
    act.type = IO_DUP;
    act.dup.oldfd = oldfd;
    submit_action(&wire_io, &act.common);
    errno = act.dup.verrno;
    return act.dup.ret;
}

int wio_dup2(int oldfd, int newfd)
{
    struct wire_io_act act;
    act.type = IO_DUP2;
    act.dup2.oldfd = oldfd;
    act.dup2.newfd = newfd;
    submit_action(&wire_io, &act.common);
    errno = act.dup2.verrno;
    return act.dup2.ret;
}

