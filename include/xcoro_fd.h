#ifndef XCORO_FD_LIB_H
#define XCORO_FD_LIB_H

void xcoro_fd_init(void);

int xcoro_fd_wait_read(int fd);
int xcoro_fd_wait_msec(int msecs);

#endif
