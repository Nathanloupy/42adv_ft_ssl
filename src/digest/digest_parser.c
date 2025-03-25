#include "commons.h"
#include "digest/digest.h"

static error_t parse_opt(int key, char *arg, struct argp_state *state)
{
	t_conf_digest *conf = state->input;
	(void)arg;

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
			// TODO: Store the string in a proper way
			break;
		case ARGP_KEY_ARG:
			if (state->arg_num >= 1)
				argp_usage(state);
			// TODO: Store the file argument
			break;
		case ARGP_KEY_END:
			if (state->arg_num < 1 && !(conf->flags & FLAG_DIGEST_STRING) && !(conf->flags & FLAG_DIGEST_ECHO))
				argp_usage(state);
			break;
		default:
			return (ARGP_ERR_UNKNOWN);
	}
	return (0);
}

static struct argp argp = {digest_options, parse_opt, "[FILE...]", "FILE... files to digest (default is stdin)", NULL, NULL, NULL};

int digest_parser(int argc, char **argv, t_conf *conf)
{
	t_conf_digest *conf_digest = (t_conf_digest *)conf;
	memset(conf_digest, 0, sizeof(t_conf_digest));
	argp_parse(&argp, argc, argv, 0, 0, conf_digest);
	return (0);
}
