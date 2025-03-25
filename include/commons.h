#pragma once

#include "libs.h"

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
}	t_handler;

typedef union u_conf {
	t_conf_digest	digest;
}	t_conf;

static const t_handler HANDLERS[] = {
	{
		.name = "md5",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = NULL
	},
	{
		.name = "sha256",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = NULL
	},
	{
		.name = NULL,
		.type = VOID,
		.parser = NULL,
		.executor = NULL
	}
};

/* MAIN - UTILS */
void	print_commands(char *command);
