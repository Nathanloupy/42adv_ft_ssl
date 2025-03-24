#pragma once

#include "commons.h"

enum e_digest_flags
{
	FLAG_DIGEST_ECHO = 1 << 0,
	FLAG_DIGEST_QUIET = 1 << 1,
	FLAG_DIGEST_REVERSE = 1 << 2,
	FLAG_DIGEST_STRING = 1 << 3
};

static struct argp_option digest_options[] = {
	{0, 'p', 0, 0, "Echo STDIN to STDOUT and append the checksum to STDOUT"},
	{0, 'q', 0, 0, "Quiet mode"},
	{0, 'r', 0, 0, "Reverse the format of the output"},
	{0, 's', "String", 0, "Print the sum of the given string"},
	{0}
};

typedef struct s_conf_digest {
	int flags;
} t_conf_digest;