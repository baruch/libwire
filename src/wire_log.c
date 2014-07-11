#include "wire_log.h"
#include "wire.h"
#include "wire_io.h"
#include "wire_lock.h"

#include <syslog.h>
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static wire_t stdout_wire;
static struct timespec str_time;
static wire_log_level_e str_level;
static char str[128];
static unsigned str_len;
static char stdout_wire_stack[4096];
static wire_lock_t lock;

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
	UNUSED(arg);

	while (1) {
		if (str_len) {
			time_t t;
			struct tm tm;
			char tm_str[32];
			size_t tm_str_len;

			t = str_time.tv_sec;
			localtime_r(&t, &tm);
			tm_str_len = strftime(tm_str, sizeof(tm_str), "%Y-%m-%d %H:%M:%S.", &tm);
			wio_write(1, tm_str, tm_str_len);

			tm_str_len = snprintf(tm_str, sizeof(tm_str), "%010lu <%s> ", (long unsigned)str_time.tv_nsec, level_to_str(str_level));
			wio_write(1, tm_str, tm_str_len);

			wio_write(1, str, str_len);
			str_len = 0;

			wire_lock_release(&lock);
		}
		wire_suspend();
	}
}

static void wire_log_stdout(wire_log_level_e level, const char *fmt, ...)
{
	va_list ap;
	struct timespec t;

	clock_gettime(CLOCK_REALTIME, &t);

	// Wait until logging is possible
	wire_lock_take(&lock);

	str_time = t;
	str_level = level;
	va_start(ap, fmt);
	str_len = vsnprintf(str, sizeof(str), fmt, ap);
	va_end(ap);

	if (str_len > sizeof(str)-1) {
		str_len = sizeof(str)-1;
	}
	str[str_len++] = '\n';

	wire_resume(&stdout_wire);
}

void wire_log_init_stdout(void)
{
	wire_lock_init(&lock);
	wire_init(&stdout_wire, "stdout logger", stdout_wire_func, NULL, stdout_wire_stack, sizeof(stdout_wire_stack));
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
