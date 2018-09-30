#!/usr/bin/env python

includes = [
        "sys/types.h",
        "sys/stat.h",
        "sys/socket.h",
        "sys/vfs.h",
        "sys/ioctl.h",
        "sys/wait.h",
        "fcntl.h",
        "unistd.h",
        "netdb.h",
        "ifaddrs.h",
        "sys/uio.h",
        "sys/mman.h",
        "dirent.h",
        "glob.h",
        "stdio.h",
        "ftw.h",
#        "syslog.h",
        ]

typedefs = [
        "typedef int (*glob_errfunc_t)(const char *epath, int eerrno)",
        "typedef int (*ftw_cb_t)(const char *fpath, const struct stat *sb, int typeflag)",
        "typedef int (*nftw_cb_t)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)",
        ]

syscalls = [
        "int creat(const char* filename, mode_t mode)",
        "int creat64(const char* filename, mode_t mode)",
        ("int open(const char *pathname, int flags, mode_t mode)", 1),
        "int close(int fd)",
        "ssize_t pread(int fd, void *buf, size_t count, off_t offset)",
        "ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)",
        "ssize_t read(int fd, void *buf, size_t count)",
        "ssize_t write(int fd, const void *buf, size_t count)",
        "int lockf(int fd, int cmd, off_t len)",
        "int lockf64(int fd, int cmd, off64_t len)",
        "int posix_fadvise(int fd, off_t offset, off_t len, int advise)",
        "int posix_fadvise64(int fd, off64_t offset, off64_t len, int advise)",
        "int posix_fallocate(int fd, off_t offset, off_t len)",
        "int posix_fallocate64(int fd, off64_t offset, off64_t len)",
        ("int fstat(int fd, struct stat *buf)",),
        ("int stat(const char *path, struct stat *buf)",),
        "off_t lseek(int fd, off_t offset, int whence)",
        "off64_t lseek64(int fd, off64_t offset, int whence)",
        "int ftruncate(int fd, off_t length)",
        "int truncate(const char* filename, off_t offset)",
        "int fallocate(int fd, int mode, off_t offset, off_t len)",
        "int fsync(int fd)",
        "void sync(void)",
        "int fdatasync(int fd)",
        "int statfs(const char *path, struct statfs *buf)",
        "int fstatfs(int fd, struct statfs *buf)",
        "int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)",
        "int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, socklen_t hostlen, char *serv, socklen_t servlen, int flags)",
        ("int ioctl(int d, unsigned long request, void *argp)", 1),
        "int getifaddrs(struct ifaddrs **ifap)",
        "ssize_t readv(int fd, const struct iovec *iov, int iovcnt)",
        "ssize_t writev(int fd, const struct iovec *iov, int iovcnt)",
        "ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)",
        "ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)",
        "void *mmap(void *addr, size_t length, int protc, int flags, int fd, off_t offset)",
        "int munmap(void *addr, size_t length)",
        "int mprotect(void* addr, size_t len, int prot)",
        "int msync(void* addr, size_t len, int flags)",
        "int madvise(void* addr, size_t len, int advice)",
        "int posix_madvise(void* addr, size_t len, int advice)",
        "int mlock(const void* addr, size_t len)",
        "int munlock(const void* addr, size_t len)",
        "int mlockall(int flags)",
        "int munlockall(void)",
#        "void* mremap(void* addr, size_t old_len, size_t new_len, int flags, ...)",
#        "int shm_open(const char* name, int oflag, mode_t mode)",
#        "int shm_unlink(const char* name)",
        "DIR *opendir(const char *name)",
        "DIR *fdopendir(int fd)",
        "int closedir(DIR *dirp)",
        "int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)",
        "struct dirent *readdir(DIR *dirp)",
        "void rewinddir(DIR* dirp)",
        "void seekdir(DIR* dirp, long int pos)",
        "long int telldir(DIR* dirp)",
        "int dirfd(DIR* dirp)",
        "int glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob)",
        "void globfree(glob_t *pglob)",
        "FILE *popen(const char *command, const char *type)",
        "int pclose(FILE *stream)",
        "int fgetc(FILE *stream)",
        "char *fgets(char *s, int size, FILE *stream)",
        "int kill(pid_t pid, int sig)",
        "int ftw(const char *dirpath, ftw_cb_t cb, int nopenfd)",
        "int nftw(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags)",
        "pid_t wait(int *status)",
        "pid_t waitpid(pid_t pid, int *status, int options)",
        "pid_t wait3(int* stat_loc, int options, struct rusage* usage)",
        "pid_t wait4(pid_t pid, int* stat_loc, int options, struct rusage* usage)",
        "int dup(int oldfd)",
        "int dup2(int oldfd, int newfd)",
#        "int pip(int pipes[2])",
        "unsigned int alarm(unsigned int seconds)",
        "unsigned int ualarm(unsigned value, unsigned interval)",
        "int chown(const char *filename, uid_t owner, gid_t group)",
        "int fchown(int fd, uid_t owner, gid_t group)",
        "int lchown(const char* filename, uid_t owner, gid_t group)",
        "int fchownat(int fd, const char* filename, uid_t owner, gid_t group, int flag)",
        "int fchdir(int fd)",
        "char* getcwd(char* buf, size_t size)",
        "int execve(const char* path, char *const* argv, char *const* envp)",
        "int execv(const char* path, char *const* argv)",
# execle, exexecl, execvp execlp -- require variable paratmeter len handling
        "int execvpe(const char* file, char *const* argv, char *const* envp)",
        "pid_t getpid(void)",
        "pid_t getppid(void)",
        "pid_t getpgrp(void)",
        "pid_t getpgid(pid_t pid)",
        "int setpgid(pid_t pid, pid_t pgid)",
        "int setpgrp(void)",
        "pid_t setsid(void)",
        "pid_t getsid(pid_t pid)",
        "uid_t getuid(void)",
        "uid_t geteuid(void)",
        "gid_t getgid(void)",
        "pid_t fork(void)",
        "int link(const char* from, const char* to)",
        "int symlink(const char* from, const char* to)",
        "ssize_t readlink(const char* path, char* buf, size_t len)",
        "int unlink(const char *name)",
        "int rmdir(const char* path)",
#        "void openlog(const char* ident, int option, int facility)",
#        "void closelog(void)",
#        "int setlogmask(int mask)",
#        "void syslog(int pri, const char* fmt, ...)",
#        "void vsyslog(int pri, const char* fmt, va_list ap)",
        ]

import re
import sys

gen_header_file = 'h' in sys.argv[1:]
gen_c_file = 'c' in sys.argv[1:]

if not gen_header_file and not gen_c_file:
    print 'Must choose either "c" or "h"'
    sys.exit(1)

if gen_header_file and gen_c_file:
    print 'Must choose only one of "c" or "h"'
    sys.exit(2)

decl_re = re.compile(r'^([A-Za-z_* 0-9]+[* ])([A-Za-z0-9_]+)\((.*)\)$')
#arg_re = re.compile(r'^ ?(?:([A-Za-z_* 0-9]+[* ])([A-Za-z0-9_]+)) ?$')
arg_re = re.compile(r'^ ?(?:([A-Za-z_* 0-9]+[* ])([A-Za-z0-9_]+)|()(\.\.\.)|(void)()) ?$')

def strip_list(l):
    if type(l) == str or type(l) == unicode:
        return l.strip()
    return map(strip_list, l)

class FuncDecl(object):
    def __init__(self, decl):
        self.special_dlsym = False
        self.special_gen = False
        if type(decl) == list or type(decl) == tuple:
            self.is_special = True
            if len(decl) > 1:
                print >>sys.stderr, 'decl', len(decl), decl
                self.special_dlsym = True
                if decl[1] == 1:
                    self.special_gen = True
            decl = decl[0]
        else:
            self.is_special = False

        m = decl_re.match(decl)
        if m is None:
            raise BaseException("Declaration '%s' failed to parse by regex" % decl)

        self.ret_type, self.func_name, self.args_full = m.groups()
        args = self.args_full.split(',')
        self.argd = []
        if len(args[0]) > 0:
            for arg in args:
                m = arg_re.match(arg)
                if m is None:
                    raise BaseException("Argument '%s' cannot be parsed by regex" % arg)
                g = (m.group(1), m.group(2))
                if g[0] is None or g[1] is None:
                    g = (m.group(3), m.group(4))
                    if g[0] is None and g[1] is None:
                        continue
                self.argd.append(g)

        self.ret_type = strip_list(self.ret_type)
        self.func_name = strip_list(self.func_name);
        self.args_full = strip_list(self.args_full)
        self.argd = strip_list(self.argd)

        if self.is_special:
            print >>sys.stderr, 'special', self.func_name, self.is_special, self.special_dlsym, self.special_gen
parsed_decl = map(lambda x: FuncDecl(x), syscalls)

def enum_name(decl):
    return 'IO_' + decl.func_name.upper()

def args_call(decl):
    parts = map(lambda arg: 'act->%s.%s' % (decl.func_name, arg[1]), decl.argd)
    return ', '.join(parts)

def args_call_2(args):
    if len(args) == 0: return ''
    parts = map(lambda arg: arg[1], args)
    return ', '.join(parts)

if gen_header_file:
    print '#ifndef WIRE_LIB_IO_GEN_H'
    print '#define WIRE_LIB_IO_GEN_H'
    print
    for inc in includes:
        print '#include <%s>' % inc
    for typedef in typedefs:
        print typedef, ";"
    for decl in parsed_decl:
        print '%s wio_%s(%s);' % (decl.ret_type, decl.func_name, decl.args_full)
    print
    print '#endif'
else:
    print '#include "wire_io_gen.h"'
    print
    print 'enum wio_type {'
    for decl in parsed_decl:
        print '\t%s,' % enum_name(decl)
    print '};'
    print
    print 'struct wire_io_act {'
    print '    struct wire_io_act_common common;'
    print '    enum wio_type type;'
    print '    union {'
    for decl in parsed_decl:
        print '        struct {'
        for arg in decl.argd:
            print '            %s %s;' % (arg[0], arg[1])
        if decl.ret_type != 'void':
            print '            %s ret;' % decl.ret_type
            print '            int verrno;'
        print '        } %s;' % decl.func_name
    print '    };'
    print '};'
    print
    print 'static void perform_action(struct wire_io_act *act)'
    print '{'
    print '    switch (act->type) {'
    for decl in parsed_decl:
        print '        case %s:' % enum_name(decl)
        if decl.ret_type != 'void':
            print '            act->%s.ret = %s(%s);' % (decl.func_name, decl.func_name, args_call(decl))
            print '            act->%s.verrno = errno;' % decl.func_name
        else:
            print '            %s(%s);' % (decl.func_name, args_call(decl))
        print '            break;'
    print '    }'
    print '}'
    print
    for decl in parsed_decl:
        print '%s wio_%s(%s)' % (decl.ret_type, decl.func_name, decl.args_full)
        print '{'
        print '    struct wire_io_act act;'
        print '    act.type = %s;' % enum_name(decl)
        for arg in decl.argd:
            print '    act.%s.%s = %s;' % (decl.func_name, arg[1], arg[1])
        print '    submit_action(&act.common);'
        if decl.ret_type != 'void':
            print '    errno = act.%s.verrno;' % decl.func_name
            print '    return act.%s.ret;' % decl.func_name
        print '}'
        print

    for decl in parsed_decl:
        if decl.is_special: continue
        print 'static %s (*orig_%s)(%s);' % (decl.ret_type, decl.func_name, decl.args_full)

    print '__attribute__((constructor)) static void wire_dlsym_init(void)'
    print '{'
    for decl in parsed_decl:
        if decl.is_special and not decl.special_dlsym: continue
        print '    orig_%s = dlsym(RTLD_NEXT, "%s");' % (decl.func_name, decl.func_name)
        print '    if (orig_%s == NULL) { fputs("Failed to get address of %s\\n", stderr); abort(); }' % (decl.func_name, decl.func_name)
    print '}'

    for decl in parsed_decl:
        if decl.is_special and not decl.special_gen: continue

        if not decl.special_gen:
            print '%s %s(%s)' % (decl.ret_type, decl.func_name, decl.args_full)
        else:
            print 'static %s gen_%s(%s)' % (decl.ret_type, decl.func_name, decl.args_full)
        print '{'
        print '    if (is_wire_thread) {'
        print '        return wio_%s(%s);' % (decl.func_name, args_call_2(decl.argd))
        print '    } else {'
        print '        return orig_%s(%s);' % (decl.func_name, args_call_2(decl.argd))
        print '    }'
        print '}'
