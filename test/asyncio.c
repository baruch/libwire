#include "wire.h"
#include "wire_fd.h"
#include "wire_stack.h"
#include "wire_io.h"

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <stdarg.h>

static wire_t task_io1;
static wire_t task_io2;
static wire_t task_io3;
static wire_t task_io4;
static wire_t task_io5;

#define LOG(msg) printf("%s:" msg "\n", filename)

static void test_vsyslog(const char* fmt, ...)
{
	va_list ap;

	va_start(ap, fmt);
	vsyslog(LOG_INFO, fmt, ap);
	va_end(ap);
}

static void io(void *arg)
{
	const char *filename = arg;

	LOG("wio_open");
	int fd = open(filename, O_CREAT|O_RDWR, 0666);
	int ret;
	(void)ret;

#if 0
	LOG("wio_fallocate");
	ret = wio_fallocate(fd, 0, 0, strlen(filename));
	assert(ret == 0);
#endif

	LOG("wio_pwrite");
	ret = wio_pwrite(fd, filename, strlen(filename), 0);
	assert(ret == (int)strlen(filename));

	LOG("wio_ftruncate");
	ret = wio_ftruncate(fd, strlen(filename));
	assert(ret == 0);

	LOG("wio_fsync");
	ret = wio_fsync(fd);
	assert(ret == 0);

	LOG("use read from dlsym");
	char buf[64];
	read(fd, buf, sizeof(buf));

	LOG("wio_pread");
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

	LOG("test syslog");
	openlog("asyncio-test", LOG_PERROR, LOG_USER);
	test_vsyslog("message %s %s", "hello", "world!");
	syslog(LOG_DEBUG, "just a message %p %s", arg, filename);
	closelog();

	LOG("Done");
}

int main()
{
	wire_thread_init();
	wire_stack_fault_detector_install();
	wire_fd_init();
	wire_io_init(16);
	wire_init(&task_io1, "io 1", io, "/tmp/a.1", WIRE_STACK_ALLOC(64*1024));
	wire_init(&task_io2, "io 2", io, "/tmp/a.2", WIRE_STACK_ALLOC(64*1024));
	wire_init(&task_io3, "io 3", io, "/tmp/a.3", WIRE_STACK_ALLOC(64*1024));
	wire_init(&task_io4, "io 4", io, "/tmp/a.4", WIRE_STACK_ALLOC(64*1024));
	wire_init(&task_io5, "io 5", io, "/tmp/a.5", WIRE_STACK_ALLOC(64*1024));
	wire_thread_run();
	return 0;
}
