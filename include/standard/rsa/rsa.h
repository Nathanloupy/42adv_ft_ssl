#pragma once

#include "commons.h"

#define RSA_OPTION_HELP 128
#define RSA_OPTION_USAGE 129
#define RSA_OPTION_INFORM 130
#define RSA_OPTION_OUTFORM 131
#define RSA_OPTION_PASSIN 132
#define RSA_OPTION_PASSOUT 133
#define RSA_OPTION_DES 134
#define RSA_OPTION_TEXT 135
#define RSA_OPTION_NOOUT 136
#define RSA_OPTION_MODULUS 137
#define RSA_OPTION_CHECK 138
#define RSA_OPTION_PUBIN 139
#define RSA_OPTION_PUBOUT 140

static struct argp_option rsa_options[] __attribute__((used)) = {
	{"inform", RSA_OPTION_INFORM, "FORMAT", 0, "Input format - default PEM (one of DER, NET or PEM)", 0},
	{"outform", RSA_OPTION_OUTFORM, "FORMAT", 0, "Output format - default PEM (one of DER, NET or PEM)", 0},
	{"in", 'i', "FILE", 0, "Input file", 0},
	{"passin", RSA_OPTION_PASSIN, "ARG", 0, "Input file pass phrase source", 0},
	{"out", 'o', "FILE", 0, "Output file", 0},
	{"passout", RSA_OPTION_PASSOUT, "ARG", 0, "Output file pass phrase source", 0},
	{"des", RSA_OPTION_DES, 0, 0, "Encrypt the generated key with DES in cbc mode", 0},
	{"text", RSA_OPTION_TEXT, 0, 0, "Print the key in text", 0},
	{"noout", RSA_OPTION_NOOUT, 0, 0, "Don't output the key", 0},
	{"modulus", RSA_OPTION_MODULUS, 0, 0, "Print the RSA key modulus", 0},
	{"check", RSA_OPTION_CHECK, 0, 0, "Verify key consistency", 0},
	{"pubin", RSA_OPTION_PUBIN, 0, 0, "Expect a public key in input file", 0},
	{"pubout", RSA_OPTION_PUBOUT, 0, 0, "Output a public key", 0},
	{"help", RSA_OPTION_HELP, 0, 0, "Give this help list", 0},
	{"usage", RSA_OPTION_USAGE, 0, 0, "Give a short usage message", 0},
	{0}
};

enum e_rsa_flags
{
	FLAG_RSA_HELP = 1 << 0,
	FLAG_RSA_USAGE = 1 << 1,
	FLAG_RSA_INPUT_FILE = 1 << 2,
	FLAG_RSA_OUTPUT_FILE = 1 << 3,
	FLAG_RSA_INFORM = 1 << 4,
	FLAG_RSA_OUTFORM = 1 << 5,
	FLAG_RSA_PASSIN = 1 << 6,
	FLAG_RSA_PASSOUT = 1 << 7,
	FLAG_RSA_DES = 1 << 8,
	FLAG_RSA_TEXT = 1 << 9,
	FLAG_RSA_NOOUT = 1 << 10,
	FLAG_RSA_MODULUS = 1 << 11,
	FLAG_RSA_CHECK = 1 << 12,
	FLAG_RSA_PUBIN = 1 << 13,
	FLAG_RSA_PUBOUT = 1 << 14,
};

enum e_rsa_format
{
	RSA_FORMAT_PEM,
	RSA_FORMAT_DER,
	RSA_FORMAT_NET,
};

typedef struct s_conf_rsa {
	int						flags;
	char					*input_file;
	char					*output_file;
	char					*passin;
	char					*passout;
	enum e_rsa_format		inform;
	enum e_rsa_format		outform;
	int						output_fd;
}	t_conf_rsa;

/* RSA - PARSER */
int		rsa_parser(int argc, char **argv, t_conf *conf);
error_t	rsa_parse_opt(int key, char *arg, struct argp_state *state);

/* RSA - EXECUTOR */
int		rsa_executor(t_conf *conf);

/* RSA - CLEANER */
void	rsa_cleaner(t_conf *conf);

/* RSA - UTILS */
void	rsa_init(t_conf_rsa *conf_rsa);
int		rsa_recoverable_error(t_conf *conf);
