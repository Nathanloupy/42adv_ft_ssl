#include "commons.h"

static struct argp genrsa_argp = {genrsa_options, genrsa_parse_opt, NULL, "Generate a RSA private key", NULL, NULL, NULL};

error_t genrsa_parse_opt(int key, char *arg, struct argp_state *state)
{
	t_conf_genrsa *conf = state->input;

	switch (key)
	{
		case 'o':
			conf->flags |= FLAG_GENRSA_OUTPUT_FILE;
			conf->output_file = arg;
			break;
		case GENRSA_OPTION_HELP:
			argp_help(&genrsa_argp, stderr, ARGP_HELP_STD_HELP, state->argv[0]);
			conf->flags |= FLAG_GENRSA_HELP;
			return (0);
		case GENRSA_OPTION_USAGE:
			argp_help(&genrsa_argp, stderr, ARGP_HELP_USAGE, state->argv[0]);
			conf->flags |= FLAG_GENRSA_USAGE;
			return (0);
		case ARGP_KEY_ARG:
				return (ARGP_ERR_UNKNOWN);
			break;
		case ARGP_KEY_END:
			break;
		default:
			return (ARGP_ERR_UNKNOWN);
	}
	return (0);
}

int genrsa_parser(int argc, char **argv, t_conf *conf)
{
	t_conf_genrsa *conf_genrsa = (t_conf_genrsa *)conf;
	memset(conf_genrsa, 0, sizeof(t_conf_genrsa));
	genrsa_init(conf_genrsa);
	return (argp_parse(&genrsa_argp, argc, argv, ARGP_NO_EXIT | ARGP_NO_HELP, 0, conf_genrsa));
}
