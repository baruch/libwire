CFLAGS=-Iinclude -g -O0
base: test/base.o src/xcoro.o src/xcoro_fd.o
	gcc -o $@ $^

.PHONY: clean
clean:
	rm -f test/base.o src/xcoro.o src/xcoro_fd.o base
