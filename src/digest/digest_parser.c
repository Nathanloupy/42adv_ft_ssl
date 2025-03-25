#include "commons.h"
#include "digest/digest.h"

static int	add_file_to_conf(t_conf_digest *conf_digest, char *file)
{
	t_list	*new;
	char	*file_copy;

	file_copy = strdup(file);
	if (!file_copy)
	{
		perror(FT_SSL_NAME);
		return (1);
	}
	new = lstnew(file_copy);
	if (!new)
	{
		perror(FT_SSL_NAME);
		free(file_copy);
		return (1);
	}
	lstadd_back(&conf_digest->files, new);
	return (0);
}

static error_t parse_opt(int key, char *arg, struct argp_state *state)
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
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1)
				argp_usage(state);
			if (add_file_to_conf(conf, arg))
				return (1);
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 1 && !(conf->flags & FLAG_DIGEST_STRING) && !(conf->flags & FLAG_DIGEST_ECHO) && !lstsize(*conf->files))
			{
				argp_usage(state);
				return (1);
			}
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
	struct argp argp = {digest_options, parse_opt, "[FILE...]", "FILE... files to digest (default is stdin)", NULL, NULL, NULL};
	return (argp_parse(&argp, argc, argv, 0, 0, conf_digest));
}
