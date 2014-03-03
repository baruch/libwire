#ifndef XCORO_FD_LIB_H
#define XCORO_FD_LIB_H

void xcoro_fd_init(void);

int xcoro_fd_wait_read(int fd);
int xcoro_fd_wait_msec(int msecs);

typedef enum xcoro_fd_mode {
	FD_MODE_NONE,
	FD_MODE_READ,
	FD_MODE_WRITE,
} xcoro_fd_mode_e;

typedef struct xcoro_fd_state {
	int fd;
	xcoro_fd_mode_e state;
} xcoro_fd_state_t;

void xcoro_fd_mode_init(xcoro_fd_state_t *state, int fd);
int xcoro_fd_mode_read(xcoro_fd_state_t *fd_state);
int xcoro_fd_mode_write(xcoro_fd_state_t *fd_state);
int xcoro_fd_mode_none(xcoro_fd_state_t *fd_state);
void xcoro_fd_wait(xcoro_fd_state_t *fd_state);

#endif
