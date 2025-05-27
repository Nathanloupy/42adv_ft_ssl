#pragma once

#define FT_SSL_NAME "ft_ssl"

#include "libs.h"
#include "list.h"
#include "errors.h"
#include "get_next_line.h"

typedef struct s_conf_digest	t_conf_digest;
typedef struct s_conf_base64	t_conf_base64;
typedef struct s_conf_des		t_conf_des;
typedef union u_conf			t_conf;

#include "digest.h"
#include "cipher.h"

enum e_handlers_types {
	VOID,
	STANDARD,
	DIGEST,
	CIPHER,
};

typedef struct s_handler {
	const char				*name;
	enum e_handlers_types	type;
	int						(*parser)(int argc, char **argv, t_conf *conf);
	int						(*executor)(t_conf *conf);
	void					(*cleaner)(t_conf *conf);
	int						(*error)(t_conf *conf);
}	t_handler;

typedef union u_conf {
	t_conf_digest	digest;
	t_conf_base64	base64;
}	t_conf;

static const t_handler HANDLERS[] = {
	{
		.name = "md5",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = md5_executor,
		.cleaner = digest_cleaner,
		.error = digest_recoverable_error
	},
	{
		.name = "sha256",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = sha256_executor,
		.cleaner = digest_cleaner,
		.error = digest_recoverable_error
	},
	{
		.name = "whirlpool",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = whirlpool_executor,
		.cleaner = digest_cleaner,
		.error = digest_recoverable_error
	},
	{
		.name = "base64",
		.type = CIPHER,
		.parser = base64_parser,
		.executor = base64_executor,
		.cleaner = base64_cleaner,
		.error = base64_recoverable_error
	},
	{
		.name = "des",
		.type = CIPHER,
		.parser = des_parser,
		.executor = des_executor,
		.cleaner = des_cleaner,
		.error = des_recoverable_error
	},
	{
		.name = "des-ecb",
		.type = CIPHER,
		.parser = des_parser,
		.executor = des_executor,
		.cleaner = des_cleaner,
		.error = des_recoverable_error
	},
	{
		.name = "des-cbc",
		.type = CIPHER,
		.parser = des_parser,
		.executor = des_executor,
		.cleaner = des_cleaner,
		.error = des_recoverable_error
	},
	{
		.name = NULL,
		.type = VOID,
		.parser = NULL,
		.executor = NULL,
		.cleaner = NULL,
		.error = NULL
	}
};

/* MAIN - UTILS */
void	print_commands(char *command, int is_invalid);
void	print_block(const unsigned char *block, size_t block_size, char format);
int		is_whitespace(char c);
