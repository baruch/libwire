CFLAGS=-Iinclude -g -O0

all: base echo_server

base: test/base.o src/xcoro.o src/xcoro_fd.o src/xcoro_task_pool.o
	gcc -o $@ $^

echo_server: test/echo_server.o src/xcoro.o src/xcoro_fd.o src/xcoro_task_pool.o
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -f test/base.o src/xcoro_task_pool.o src/xcoro.o src/xcoro_fd.o base echo_server
