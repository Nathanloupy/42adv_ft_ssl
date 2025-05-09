#pragma once

#include "commons.h"

#define ROTL(x, c) ((x << c) | (x >> (32 - c)))
#define ROTR(x, c) ((x >> c) | (x << (32 - c)))

#define DIGEST_NAME_MD5 "MD5"
#define DIGEST_NAME_SHA256 "SHA256"
#define DIGEST_NAME_WHIRLPOOL "WHIRLPOOL"

enum e_digest_flags
{
	FLAG_DIGEST_ECHO = 1 << 0,
	FLAG_DIGEST_QUIET = 1 << 1,
	FLAG_DIGEST_REVERSE = 1 << 2,
	FLAG_DIGEST_STRING = 1 << 3,
	FLAG_DIGEST_HELP = 1 << 4,
	FLAG_DIGEST_USAGE = 1 << 5,
};

enum e_digest_input_type
{
	INPUT_DIGEST_TYPE_STDIN,
	INPUT_DIGEST_TYPE_STRING,
	INPUT_DIGEST_TYPE_FILE
};

typedef struct s_conf_digest {
	int		recoverable_error;
	int		flags;
	char	*string;
	t_list	*files;
	size_t	stdin_size;
}	t_conf_digest;

#define DIGEST_OPTION_HELP 128
#define DIGEST_OPTION_USAGE 129

static struct argp_option digest_options[] __attribute__((used)) = {
	{0, 'p', 0, 0, "Echo STDIN to STDOUT and append the checksum to STDOUT", 0},
	{0, 'q', 0, 0, "Quiet mode", 0},
	{0, 'r', 0, 0, "Reverse the format of the output", 0},
	{0, 's', "String", 0, "Print the sum of the given string", 0},
	{"help", DIGEST_OPTION_HELP, 0, 0, "Give this help list", 0},
	{"usage", DIGEST_OPTION_USAGE, 0, 0, "Give a short usage message", 0},
	{0}
};

int		digest_parser(int argc, char **argv, t_conf *conf);
void	digest_cleaner(t_conf *conf);
int		digest_recoverable_error(t_conf *conf);
void	digest_print_result(char *result, t_conf_digest *conf_digest, const char *digester_name, char *source_name, int input_type);
int		digest_add_to_input(char **input, unsigned char *buffer, size_t bytes_read, size_t total_bytes);
error_t	digest_parse_opt(int key, char *arg, struct argp_state *state);

#include "md5/md5.h"
#include "sha256/sha256.h"
#include "whirlpool/whirlpool.h"