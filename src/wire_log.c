#include "wire_log.h"
#include "wire.h"
#include "wire_io.h"
#include "wire_semaphore.h"

#include <syslog.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

#define NUM_LINES 16

static void null_wire_log(wire_log_level_e level, const char *fmt, ...)
{
	UNUSED(level);

	va_list ap;

	// NOTE: This will block the process!
	va_start(ap, fmt);
	vprintf(fmt, ap);
	va_end(ap);
}

void (*wire_log)(wire_log_level_e level, const char *fmt, ...) = null_wire_log;

struct log_line {
	struct timespec t;
	wire_log_level_e level;
	unsigned str_len;
	char str[128];
};

static wire_t stdout_wire;
static char stdout_wire_stack[4096];
static wire_sem_t sem;
static struct log_line lines[NUM_LINES];
static int cur_log_line;
static int cur_write_line;

static const char *level_to_str(wire_log_level_e level)
{
	switch (level) {
		case WLOG_FATAL: return "FATAL"; break;
		case WLOG_CRITICAL: return "CRIT"; break;
		case WLOG_ERR: return "ERR"; break;
		case WLOG_WARNING: return "WARN"; break;
		case WLOG_NOTICE: return "NOTICE"; break;
		case WLOG_INFO: return "INFO"; break;
		case WLOG_DEBUG: return "DEBUG"; break;
	}
	return "UNWKNON";
}

static void stdout_wire_func(void *arg)
{
	int fd = (long int)arg;

	while (1) {
		struct log_line *line = &lines[cur_write_line];
		if (line->str_len) {
			time_t t;
			struct tm tm;
			char tm_str[24];
			size_t tm_str_len;
			char next_str[24];
			size_t next_str_len;
			struct iovec iov[3];

			t = line->t.tv_sec;
			localtime_r(&t, &tm);
			tm_str_len = strftime(tm_str, sizeof(tm_str), "%Y-%m-%d %H:%M:%S.", &tm);

			next_str_len = snprintf(next_str, sizeof(next_str), "%09lu <%s> ", (long unsigned)line->t.tv_nsec, level_to_str(line->level));

			iov[0].iov_base = tm_str;
			iov[0].iov_len = tm_str_len;
			iov[1].iov_base = next_str;
			iov[1].iov_len = next_str_len;
			iov[2].iov_base = line->str;
			iov[2].iov_len = line->str_len;
			wio_writev(fd, iov, 3);
			line->str_len = 0;

			wire_sem_release(&sem);
			if (++cur_write_line == NUM_LINES)
				cur_write_line = 0;
		} else {
			wire_suspend();
		}
	}
}

static void wire_log_stdout(wire_log_level_e level, const char *fmt, ...)
{
	va_list ap;
	struct timespec t;

	clock_gettime(CLOCK_REALTIME, &t);

	// Wait until logging is possible
	wire_sem_take(&sem);
	struct log_line *line = &lines[cur_log_line];
	if (++cur_log_line == NUM_LINES)
		cur_log_line = 0;

	line->t = t;
	line->level = level;
	va_start(ap, fmt);
	line->str_len = vsnprintf(line->str, sizeof(line->str), fmt, ap);
	va_end(ap);

	if (line->str_len > sizeof(line->str)-1) {
		line->str_len = sizeof(line->str)-1;
	}
	line->str[line->str_len++] = '\n';

	wire_resume(&stdout_wire);
}

void wire_log_init_stderr(void)
{
	wire_sem_init(&sem, NUM_LINES);
	wire_init(&stdout_wire, "stderr logger", stdout_wire_func, (void*)2, stdout_wire_stack, sizeof(stdout_wire_stack));
	wire_log = wire_log_stdout;
}

void wire_log_init_stdout(void)
{
	wire_sem_init(&sem, NUM_LINES);
	wire_init(&stdout_wire, "stdout logger", stdout_wire_func, (void*)1, stdout_wire_stack, sizeof(stdout_wire_stack));
	wire_log = wire_log_stdout;
}

static void wire_log_syslog(wire_log_level_e level, const char *fmt, ...)
{
	va_list ap;
	int prio = LOG_INFO;

	switch (level) {
		case WLOG_FATAL: prio = LOG_EMERG; break;
		case WLOG_CRITICAL: prio = LOG_CRIT; break;
		case WLOG_ERR: prio = LOG_ERR; break;
		case WLOG_WARNING: prio = LOG_WARNING; break;
		case WLOG_NOTICE: prio = LOG_NOTICE; break;
		case WLOG_INFO: prio = LOG_INFO; break;
		case WLOG_DEBUG: prio = LOG_DEBUG; break;
	}

	va_start(ap, fmt);
	vsyslog(prio, fmt, ap);
	va_end(ap);
}

void wire_log_init_syslog(const char *ident, int option, int facility)
{
	openlog(ident, option|LOG_NDELAY, facility);
	wire_log = wire_log_syslog;
}
