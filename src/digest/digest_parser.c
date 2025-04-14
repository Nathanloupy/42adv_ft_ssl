#include "commons.h"

static struct argp digest_argp = {digest_options, digest_parse_opt, "[FILE...]", "FILE... files to digest (default is stdin)", NULL, NULL, NULL};

static int	add_file_to_conf(t_conf_digest *conf_digest, char *file)
{
	t_list	*new;
	char	*file_copy;

	file_copy = strdup(file);
	if (!file_copy)
		return (perror_int());
	new = lstnew(file_copy);
	if (!new)
	{
		free(file_copy);
		return (perror_int());
	}
	lstadd_back(&conf_digest->files, new);
	return (0);
}

error_t digest_parse_opt(int key, char *arg, struct argp_state *state)
{
	t_conf_digest *conf = state->input;

	switch (key)
	{
		case 'p':
			conf->flags |= FLAG_DIGEST_ECHO;
			break;
		case 'q':
			conf->flags |= FLAG_DIGEST_QUIET;
			break;
		case 'r':
			conf->flags |= FLAG_DIGEST_REVERSE;
			break;
		case 's':
			conf->flags |= FLAG_DIGEST_STRING;
			conf->string = arg;
			break;
		case DIGEST_OPTION_HELP:
			argp_help(&digest_argp, stderr, ARGP_HELP_STD_HELP, state->argv[0]);
			conf->flags |= FLAG_DIGEST_HELP;
			return (0);
		case DIGEST_OPTION_USAGE:
			argp_help(&digest_argp, stderr, ARGP_HELP_USAGE, state->argv[0]);
			conf->flags |= FLAG_DIGEST_USAGE;
			return (0);
		case ARGP_KEY_ARG:
			if (add_file_to_conf(conf, arg))
				return (1);
			break;
		case ARGP_KEY_END:
			break;
		default:
			return (ARGP_ERR_UNKNOWN);
	}
	return (0);
}

int digest_parser(int argc, char **argv, t_conf *conf)
{
	t_conf_digest *conf_digest = (t_conf_digest *)conf;
	memset(conf_digest, 0, sizeof(t_conf_digest));
	return (argp_parse(&digest_argp, argc, argv, ARGP_NO_EXIT | ARGP_NO_HELP, 0, conf_digest));
}
