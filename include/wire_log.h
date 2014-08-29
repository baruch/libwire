#ifndef WIRE_LOG_LIB_H
#define WIRE_LOG_LIB_H

/** @file
 * Common logging for all of libwire.
 */

/** @defgroup Log Logging framework
 *
 * Implement a logging framework that can be used for error and debug logging
 * for both libwire itself and any application built on top of it.
 */
/// @{

typedef enum wire_log_level {
	WLOG_DEBUG,
	WLOG_INFO,
	WLOG_NOTICE,
	WLOG_WARNING,
	WLOG_ERR,
	WLOG_CRITICAL,
	WLOG_FATAL,
} wire_log_level_e;

/** Function callback to implement the logging.
 */
extern void (*wire_log)(wire_log_level_e level, const char *fmt, ...);

/** Initialize logging to log to stdout asynchronously. */
void wire_log_init_stdout(void);

/** Initialize logging to log to stderr asynchronously. */
void wire_log_init_stderr(void);

/** Initialize logging to log to syslog asynchronously. */
void wire_log_init_syslog(const char *ident, int option, int facility);

/// @}

#endif
