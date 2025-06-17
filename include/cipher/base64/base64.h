#pragma once

#include "commons.h"

#define BASE64_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

#define BASE64_BUFFER_SIZE 1024
#define BASE64_BLOCK_SIZE 64

#define BASE64_OPTION_HELP 128
#define BASE64_OPTION_USAGE 129

static struct argp_option base64_options[] __attribute__((used)) = {
	{0, 'd', 0, 0, "decode mode", 0},
	{0, 'e', 0, 0, "encode mode", 0},
	{0, 'i', "FILE", 0, "input file", 0},
	{0, 'o', "FILE", 0, "output file", 0},
	{"help", BASE64_OPTION_HELP, 0, 0, "Give this help list", 0},
	{"usage", BASE64_OPTION_USAGE, 0, 0, "Give a short usage message", 0},
	{0}
};

enum e_base64_flags
{
	FLAG_BASE64_DECODE = 1 << 0,
	FLAG_BASE64_ENCODE = 1 << 1,
	FLAG_BASE64_INPUT_FILE = 1 << 2,
	FLAG_BASE64_OUTPUT_FILE = 1 << 3,
	FLAG_BASE64_HELP = 1 << 4,
	FLAG_BASE64_USAGE = 1 << 5,
};

typedef struct s_conf_base64 {
	int		flags;
	char	*input_file;
	int		input_fd;
	char	*output_file;
	int		output_fd;
}	t_conf_base64;


/* BASE64 - PARSER */
int		base64_parser(int argc, char **argv, t_conf *conf);
error_t	base64_parse_opt(int key, char *arg, struct argp_state *state);

/* BASE64 - ENCODE */
char	*base64_encode(const char *buffer, size_t size, size_t *output_size);
int		base64_write_encoded(const char *output, size_t output_size);

/* BASE64 - DECODE */
int		base64_check_encoded(const char *buffer, size_t size);
char	*base64_decode(const char *buffer, size_t size, size_t *output_size);

/* BASE64 - CLEANER */
void	base64_cleaner(t_conf *conf);

/* BASE64 - EXECUTOR */
int		base64_executor(t_conf *conf);

/* BASE64 - UTILS */
void	base64_init(t_conf_base64 *conf_base64);
int		base64_recoverable_error(t_conf *conf);
void	print_bits(void *buffer, size_t size);
