#pragma once

#include "commons.h"

#define GENRSA_OPTION_HELP 128
#define GENRSA_OPTION_USAGE 129
#define GENRSA_PRIME_PROBABILITY 0.999
#define GENRSA_BIT_LENGTH 64
#define GENRSA_MAX_KEY_GENERATION_ATTEMPTS 50
#define GENRSA_MAX_PRIME_CHOICE_ATTEMPTS 100000
#define GENRSA_MIN_PRIME_VALUE 10000
#define GENRSA_MAX_PRIME_VALUE (1ULL << 31)

#define GENRSA_RSA_VERSION 0
#define GENRSA_RSA_E 65537

static struct argp_option genrsa_options[] __attribute__((used)) = {
	{"out", 'o', "FILE", 0, "Output the key to specified FILE", 0},
	{"help", GENRSA_OPTION_HELP, 0, 0, "Give this help list", 0},
	{"usage", GENRSA_OPTION_USAGE, 0, 0, "Give a short usage message", 0},
	{0}
};

enum e_genrsa_flags
{
	FLAG_GENRSA_OUTPUT_FILE = 1 << 0,
	FLAG_GENRSA_HELP = 1 << 1,
	FLAG_GENRSA_USAGE = 1 << 2,
};

typedef struct s_conf_genrsa {
	int		flags;
	char	*output_file;
	int		output_fd;
}	t_conf_genrsa;

typedef struct s_rsa_key {
	u_int64_t	version;
	u_int64_t	n;
	u_int64_t	e;
	u_int64_t	d;
	u_int64_t	p;
	u_int64_t	q;
	u_int64_t	d_p;
	u_int64_t	d_q;
	u_int64_t	q_inv;
}	t_rsa_key;

/* GENRSA - PARSER */
int		genrsa_parser(int argc, char **argv, t_conf *conf);
error_t	genrsa_parse_opt(int key, char *arg, struct argp_state *state);

/* GENRSA - EXECUTOR */
int		genrsa_executor(t_conf *conf);

/* GENRSA - CLEANER */
void	genrsa_cleaner(t_conf *conf);

/* GENRSA - UTILS */
void	genrsa_init(t_conf_genrsa *conf_genrsa);
int		genrsa_recoverable_error(t_conf *conf);

/* GENRSA - GENERATE KEY */
int		generate_rsa_key(t_rsa_key *rsa_key);