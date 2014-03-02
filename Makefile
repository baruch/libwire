CFLAGS=-Iinclude -g -O0

all: base echo_server

base: test/base.o src/xcoro.o src/xcoro_fd.o
	gcc -o $@ $^

echo_server: test/echo_server.o src/xcoro.o src/xcoro_fd.o
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -f test/base.o src/xcoro.o src/xcoro_fd.o base echo_server
