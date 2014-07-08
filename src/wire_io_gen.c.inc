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
    }
}

int wio_open(const char *pathname, int flags, mode_t mode)
{
    struct wire_io_act act;
    act.type = IO_OPEN;
    act.open.pathname = pathname;
    act.open.flags = flags;
    act.open.mode = mode;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.open.ret < 0)
        errno = act.open.verrno;
    return act.open.ret;
}

int wio_close(int fd)
{
    struct wire_io_act act;
    act.type = IO_CLOSE;
    act.close.fd = fd;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.close.ret < 0)
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
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.pread.ret < 0)
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
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.pwrite.ret < 0)
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
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.read.ret < 0)
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
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.write.ret < 0)
        errno = act.write.verrno;
    return act.write.ret;
}

int wio_fstat(int fd, struct stat *buf)
{
    struct wire_io_act act;
    act.type = IO_FSTAT;
    act.fstat.fd = fd;
    act.fstat.buf = buf;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.fstat.ret < 0)
        errno = act.fstat.verrno;
    return act.fstat.ret;
}

int wio_stat(const char *path, struct stat *buf)
{
    struct wire_io_act act;
    act.type = IO_STAT;
    act.stat.path = path;
    act.stat.buf = buf;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.stat.ret < 0)
        errno = act.stat.verrno;
    return act.stat.ret;
}

int wio_ftruncate(int fd, off_t length)
{
    struct wire_io_act act;
    act.type = IO_FTRUNCATE;
    act.ftruncate.fd = fd;
    act.ftruncate.length = length;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.ftruncate.ret < 0)
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
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.fallocate.ret < 0)
        errno = act.fallocate.verrno;
    return act.fallocate.ret;
}

int wio_fsync(int fd)
{
    struct wire_io_act act;
    act.type = IO_FSYNC;
    act.fsync.fd = fd;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.fsync.ret < 0)
        errno = act.fsync.verrno;
    return act.fsync.ret;
}

int wio_statfs(const char *path, struct statfs *buf)
{
    struct wire_io_act act;
    act.type = IO_STATFS;
    act.statfs.path = path;
    act.statfs.buf = buf;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.statfs.ret < 0)
        errno = act.statfs.verrno;
    return act.statfs.ret;
}

int wio_fstatfs(int fd, struct statfs *buf)
{
    struct wire_io_act act;
    act.type = IO_FSTATFS;
    act.fstatfs.fd = fd;
    act.fstatfs.buf = buf;
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.fstatfs.ret < 0)
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
    wakeup_fd_listener();
    submit_action(&wire_io, &act.common);
    if (act.getaddrinfo.ret < 0)
        errno = act.getaddrinfo.verrno;
    return act.getaddrinfo.ret;
}

