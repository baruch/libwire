#ifndef WIRE_TEST_UTILS_H
#define WIRE_TEST_UTILS_H

void set_nonblock(int fd);
void set_reuse(int fd);
int socket_setup(unsigned short port);

#endif
