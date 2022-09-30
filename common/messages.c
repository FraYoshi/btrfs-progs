/*
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public
 * License v2 as published by the Free Software Foundation.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 021110-1307, USA.
 */

#include <stdio.h>
#include <stdarg.h>
#include "common/messages.h"
#include "common/utils.h"

static const char *common_error_string[] = {
	[ERROR_MSG_MEMORY]	= "not enough memory",
};

__attribute__ ((format (printf, 1, 2)))
void __btrfs_warning(const char *fmt, ...)
{
	va_list args;

	fputs("WARNING: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
}

__attribute__ ((format (printf, 1, 2)))
void __btrfs_error(const char *fmt, ...)
{
	va_list args;

	fputs("ERROR: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);
}

__attribute__ ((format (printf, 2, 3)))
int __btrfs_warning_on(int condition, const char *fmt, ...)
{
	va_list args;

	if (!condition)
		return 0;

	fputs("WARNING: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);

	return 1;
}

__attribute__ ((format (printf, 2, 3)))
int __btrfs_error_on(int condition, const char *fmt, ...)
{
	va_list args;

	if (!condition)
		return 0;

	fputs("ERROR: ", stderr);
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
	fputc('\n', stderr);

	return 1;
}

__attribute__ ((format (printf, 1, 2)))
void internal_error(const char *fmt, ...)
{
	va_list vargs;

	va_start(vargs, fmt);
	fputs("INTERNAL ERROR: ", stderr);
	vfprintf(stderr, fmt, vargs);
	va_end(vargs);
	fputc('\n', stderr);

#ifndef BTRFS_DISABLE_BACKTRACE
	print_trace();
#endif
}

/*
 * Print a message according to the global verbosity level.
 *
 * level: minimum verbose level at which the message will be printed
 */
__attribute__ ((format (printf, 2, 3)))
void pr_verbose(int level, const char *fmt, ...)
{
	va_list args;

	if (bconf.verbose == BTRFS_BCONF_QUIET || level == BTRFS_BCONF_QUIET)
		return;

	if (bconf.verbose < level)
		return;

	va_start(args, fmt);
	vfprintf(stdout, fmt, args);
	va_end(args);
}

/* Print common error message with optional data, appended after the generic text */
__attribute__ ((format (printf, 2, 3)))
void error_msg(enum common_error error, const char *msg, ...)
{
	const char *str = common_error_string[error];

	if (msg) {
		va_list args;

		va_start(args, msg);
		fprintf(stderr, "ERROR: %s: ", str);
		vfprintf(stderr, msg, args);
		va_end(args);
		fputc('\n', stderr);
	} else {
		fprintf(stderr, "ERROR: %s\n", str);
	}
}
