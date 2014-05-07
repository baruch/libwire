#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_io.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>

static wire_thread_t wire_main;
static wire_t task_io1;
static wire_t task_io2;
static wire_t task_io3;
static wire_t task_io4;
static wire_t task_io5;

static int count;

#define LOG(msg) printf("%s:" msg "\n", filename)

static void io(void *arg)
{
	const char *filename = arg;

	count++;

	LOG("wio_open");
	int fd = wio_open(filename, O_CREAT|O_RDWR, 0666);

	LOG("wio_pwrite");
	int ret = wio_pwrite(fd, filename, strlen(filename), 0);
	assert(ret == (int)strlen(filename));

	LOG("wio_pread");
	char buf[64];
	ret = wio_pread(fd, buf, sizeof(buf), 0);
	assert(ret == (int)strlen(filename));
	assert(memcmp(buf, filename, strlen(filename)) == 0);

	LOG("wio_ftruncate");
	ret = wio_ftruncate(fd, 0);
	assert(ret == 0);

	LOG("wio_fstat");
	struct stat st;
	ret = wio_fstat(fd, &st);
	assert(ret == 0);
	assert(st.st_size == 0);

	LOG("wio_close");
	wio_close(fd);

	LOG("Done");

	count--;
	if (count == 0) {
		wire_io_shutdown();
	}
}

int main()
{
	wire_thread_init(&wire_main);
	wire_fd_init();
	wire_io_init(1);
	wire_init(&task_io1, "io 1", io, "/tmp/a.1", WIRE_STACK_ALLOC(4096));
	wire_init(&task_io2, "io 2", io, "/tmp/a.2", WIRE_STACK_ALLOC(4096));
	wire_init(&task_io3, "io 3", io, "/tmp/a.3", WIRE_STACK_ALLOC(4096));
	wire_init(&task_io4, "io 4", io, "/tmp/a.4", WIRE_STACK_ALLOC(4096));
	wire_init(&task_io5, "io 5", io, "/tmp/a.5", WIRE_STACK_ALLOC(4096));
	wire_thread_run();
	return 0;
}
