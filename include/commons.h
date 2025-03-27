#pragma once

#define FT_SSL_NAME "ft_ssl"

#include "libs.h"
#include "list.h"
#include "errors.h"

typedef struct s_conf_digest	t_conf_digest;
typedef union u_conf			t_conf;

#include "digest.h"

enum e_handlers_types {
	DIGEST,
	CIPHER,
	VOID,
};

typedef struct s_handler {
	const char				*name;
	enum e_handlers_types	type;
	int						(*parser)(int argc, char **argv, t_conf *conf);
	int						(*executor)(t_conf *conf);
	void					(*cleaner)(t_conf *conf);
}	t_handler;

typedef union u_conf {
	t_conf_digest	digest;
}	t_conf;

static const t_handler HANDLERS[] = {
	{
		.name = "md5",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = md5_executor,
		.cleaner = digest_cleaner
	},
	{
		.name = "sha256",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = sha256_executor,
		.cleaner = digest_cleaner
	},
	{
		.name = NULL,
		.type = VOID,
		.parser = NULL,
		.executor = NULL,
		.cleaner = NULL
	}
};

/* MAIN - UTILS */
void	print_commands(char *command);
void	print_block(const unsigned char *block, size_t block_size, char format);
