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
        ]

typedefs = [
        "typedef int (*glob_errfunc_t)(const char *epath, int eerrno)",
        "typedef int (*ftw_cb_t)(const char *fpath, const struct stat *sb, int typeflag)",
        "typedef int (*nftw_cb_t)(const char *fpath, const struct stat *sb, int typeflag, struct FTW *ftwbuf)",
        ]

syscalls = [
        "int open(const char *pathname, int flags, mode_t mode)",
        "int close(int fd)",
        "ssize_t pread(int fd, void *buf, size_t count, off_t offset)",
        "ssize_t pwrite(int fd, const void *buf, size_t count, off_t offset)",
        "ssize_t read(int fd, void *buf, size_t count)",
        "ssize_t write(int fd, const void *buf, size_t count)",
        "int fstat(int fd, struct stat *buf)",
        "int stat(const char *path, struct stat *buf)",
        "int ftruncate(int fd, off_t length)",
        "int fallocate(int fd, int mode, off_t offset, off_t len)",
        "int fsync(int fd)",
        "int statfs(const char *path, struct statfs *buf)",
        "int fstatfs(int fd, struct statfs *buf)",
        "int getaddrinfo(const char *node, const char *service, const struct addrinfo *hints, struct addrinfo **res)",
        "int getnameinfo(const struct sockaddr *sa, socklen_t salen, char *host, size_t hostlen, char *serv, size_t servlen, int flags)",
        "int ioctl(int d, unsigned long request, void *argp)",
        "int getifaddrs(struct ifaddrs **ifap)",
        "ssize_t readv(int fd, const struct iovec *iov, int iovcnt)",
        "ssize_t writev(int fd, const struct iovec *iov, int iovcnt)",
        "ssize_t preadv(int fd, const struct iovec *iov, int iovcnt, off_t offset)",
        "ssize_t pwritev(int fd, const struct iovec *iov, int iovcnt, off_t offset)",
        "void *mmap(void *addr, size_t length, int protc, int flags, int fd, off_t offset)",
        "int munmap(void *addr, size_t length)",
        "DIR *opendir(const char *name)",
        "DIR *fdopendir(int fd)",
        "int closedir(DIR *dirp)",
        "int readdir_r(DIR *dirp, struct dirent *entry, struct dirent **result)",
        "struct dirent *readdir(DIR *dirp)",
        "int read_file_content(const char *filename, char *buf, size_t bufsz)",
        "int glob(const char *pattern, int flags, glob_errfunc_t errfunc, glob_t *pglob)",
        "void globfree(glob_t *pglob)",
        "FILE *popen(const char *command, const char *type)",
        "int pclose(FILE *stream)",
        "int fgetc(FILE *stream)",
        "char *fgets(char *s, int size, FILE *stream)",
        "int spawn(char **args, int *stdin_fd, int *stdout_fd, int *stderr_fd)",
        "int kill(pid_t pid, int sig)",
        "int ftw(const char *dirpath, ftw_cb_t cb, int nopenfd)",
        "int nftw(const char *dirpath, nftw_cb_t cb, int nopenfd, int flags)",
        "pid_t wait(int *status)",
        "pid_t waitpid(pid_t pid, int *status, int options)",
        "int dup(int oldfd)",
        "int dup2(int oldfd, int newfd)",
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
arg_re = re.compile(r'^ ?([A-Za-z_* 0-9]+[* ])([A-Za-z0-9_]+) ?$')

def strip_list(l):
    if type(l) == str or type(l) == unicode:
        return l.strip()
    return map(strip_list, l)

def parse_decl(decl):
    m = decl_re.match(decl)
    if m is None:
        raise BaseException("Declaration '%s' failed to parse by regex" % decl)

    ret_type, func_name, args_full = m.groups()
    args = args_full.split(',')
    argd = []
    if len(args[0]) > 0:
        for arg in args:
            m = arg_re.match(arg)
            if m is None:
                raise BaseException("Argument '%s' cannot be parsed by regex" % arg)
            argd.append(m.groups())
    ret = (ret_type, func_name, args_full, argd)
    return strip_list(ret)

parsed_decl = map(parse_decl, syscalls)

def enum_name(decl):
    return 'IO_' + decl[1].upper()

def args_call(decl):
    parts = map(lambda arg: 'act->%s.%s' % (decl[1], arg[1]), decl[3])
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
        print '%s wio_%s(%s);' % (decl[0], decl[1], decl[2])
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
        for arg in decl[3]:
            print '            %s %s;' % (arg[0], arg[1])
        if decl[0] != 'void':
            print '            %s ret;' % decl[0]
            print '            int verrno;'
        print '        } %s;' % decl[1]
    print '    };'
    print '};'
    print
    print 'static void perform_action(struct wire_io_act *act)'
    print '{'
    print '    switch (act->type) {'
    for decl in parsed_decl:
        print '        case %s:' % enum_name(decl)
        if decl[0] != 'void':
            print '            act->%s.ret = %s(%s);' % (decl[1], decl[1], args_call(decl))
            print '            act->%s.verrno = errno;' % decl[1]
        else:
            print '            %s(%s);' % (decl[1], args_call(decl))
        print '            break;'
    print '    }'
    print '}'
    print
    for decl in parsed_decl:
        print '%s wio_%s(%s)' % (decl[0], decl[1], decl[2])
        print '{'
        print '    struct wire_io_act act;'
        print '    act.type = %s;' % enum_name(decl)
        for arg in decl[3]:
            print '    act.%s.%s = %s;' % (decl[1], arg[1], arg[1])
        print '    submit_action(&wire_io, &act.common);'
        if decl[0] != 'void':
            print '    errno = act.%s.verrno;' % decl[1]
            print '    return act.%s.ret;' % decl[1]
        print '}'
        print
