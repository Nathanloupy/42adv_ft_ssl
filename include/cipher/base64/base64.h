#pragma once

#include "commons.h"

#define BASE64_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

static struct argp_option base64_options[] __attribute__((used)) = {
	{0, 'd', 0, 0, "decode mode", 0},
	{0, 'e', 0, 0, "encode mode", 0},
	{0, 'i', "FILE", 0, "input file", 0},
	{0, 'o', "FILE", 0, "output file", 0},
	{0}
};

enum e_base64_flags
{
	FLAG_BASE64_DECODE = 1 << 0,
	FLAG_BASE64_ENCODE = 1 << 1,
	FLAG_BASE64_INPUT_FILE = 1 << 2,
	FLAG_BASE64_OUTPUT_FILE = 1 << 3,
};

typedef struct s_conf_base64 {
	int		flags;
	char	*input_file;
	char	*output_file;
}	t_conf_base64;

typedef struct s_base64 {
	int		flags;
	char	*input_file;
	char	*output_file;
}	t_base64;
