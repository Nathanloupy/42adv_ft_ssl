#pragma once

#include "commons.h"

#define DES_BUFFER_SIZE 1024

#define DES_OPTION_HELP 128
#define DES_OPTION_USAGE 129

static struct argp_option des_options[] __attribute__((used)) = {
	{0, 'a', 0, 0, "Base64 encode/decode, depending on encryption flag", 0},
	{0, 'd', 0, 0, "Decrypt mode", 0},
	{0, 'e', 0, 0, "Encrypt mode", 0},
	{0, 'i', "FILE", 0, "Input file", 0},
	{0, 'o', "FILE", 0, "Output file", 0},
	{0, 'k', "KEY", 0, "Raw key, in hex", 0},
	{0, 's', "SALT", 0, "Salt, in hex", 0},
	{0, 'p', "PASSPHRASE", 0, "Passphrase source, in ascii", 0},
	{0, 'v', "IV", 0, "Initialization vector, in hex", 0},	
	{"help", DES_OPTION_HELP, 0, 0, "Give this help list", 0},
	{"usage", DES_OPTION_USAGE, 0, 0, "Give a short usage message", 0},
	{0}
};

enum e_des_flags
{
	FLAG_DES_BASE64 = 1 << 0,
	FLAG_DES_ENCRYPT = 1 << 1,
	FLAG_DES_DECRYPT = 1 << 2,
	FLAG_DES_INPUT_FILE = 1 << 3,
	FLAG_DES_OUTPUT_FILE = 1 << 4,
	FLAG_DES_KEY = 1 << 5,
	FLAG_DES_SALT = 1 << 6,
	FLAG_DES_PASSPHRASE = 1 << 7,
	FLAG_DES_IV = 1 << 8,
	FLAG_DES_HELP = 1 << 9,
	FLAG_DES_USAGE = 1 << 10,
};

enum e_des_cipher_mode
{
	DES_CBC,
	DES_ECB,
};

typedef struct s_conf_des {
	enum e_des_cipher_mode	mode;
	int						flags;
	char					*input_file;
	int						input_fd;
	char					*output_file;
	int						output_fd;
	char					*key;
	char					*salt;
	char					*passphrase;
	char					*iv;
}	t_conf_des;

typedef struct s_exec_des {
	enum e_des_cipher_mode	mode;
	u_int64_t				key;
	u_int64_t				iv;
	char					*input_buffer;
	size_t					input_buffer_size;
	char					*output_buffer;
	size_t					output_buffer_size;
}	t_exec_des;

typedef struct s_des {
	u_int64_t main_key;
	u_int64_t round_keys[16];
	u_int64_t block;
	u_int64_t block_permuted;
	u_int64_t final_round_output;
	u_int64_t result;
}	t_des;

/* DES - PARSER */
int		des_parser(int argc, char **argv, t_conf *conf);
error_t	des_parse_opt(int key, char *arg, struct argp_state *state);

/* DES - CLEANER */
void	des_cleaner(t_conf *conf);

/* DES - EXECUTOR */
int		des_executor(t_conf *conf);

/* DES - UTILS */
void	des_init(t_conf_des *conf_des);
void	des_set_mode(t_conf_des *conf_des, char *command);
int		des_recoverable_error(t_conf *conf);

/* DES - ALGORITHM */
u_int64_t	des_cipher_block(u_int64_t block, u_int64_t key);
u_int64_t	des_decipher_block(u_int64_t block, u_int64_t key);
void		des_round_keys_generation(t_des *des);
void		des_reverse_round_keys(t_des *des);
void		des_initial_permutation(t_des *des);
void		des_rounds(t_des *des);
void		des_final_permutation(t_des *des);