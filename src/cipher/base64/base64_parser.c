#include "commons.h"

static struct argp base64_argp = {base64_options, base64_parse_opt, "", "Base64 encode or decode FILE, or standard input, to standard output.", NULL, NULL, NULL};

error_t base64_parse_opt(int key, char *arg, struct argp_state *state)
{
	t_conf_base64 *conf = state->input;

	switch (key)
	{
		case 'd':
			conf->flags |= FLAG_BASE64_DECODE;
			break;
		case 'e':
			conf->flags |= FLAG_BASE64_ENCODE;
			break;
		case 'i':
			conf->flags |= FLAG_BASE64_INPUT_FILE;
			conf->input_file = arg;
			break;
		case 'o':
			conf->flags |= FLAG_BASE64_OUTPUT_FILE;
			conf->output_file = arg;
			break;
		case BASE64_OPTION_HELP:
			argp_help(&base64_argp, stderr, ARGP_HELP_STD_HELP, state->argv[0]);
			conf->flags |= FLAG_BASE64_HELP;
			return (0);
		case BASE64_OPTION_USAGE:
			argp_help(&base64_argp, stderr, ARGP_HELP_USAGE, state->argv[0]);
			conf->flags |= FLAG_BASE64_USAGE;
			return (0);
		case ARGP_KEY_ARG:
			return (ARGP_ERR_UNKNOWN);
		case ARGP_KEY_END:
			break;
		default:
			return (ARGP_ERR_UNKNOWN);
	}
	return (0);
}

int base64_parser(int argc, char **argv, t_conf *conf)
{
	t_conf_base64 *conf_base64 = (t_conf_base64 *)conf;
	memset(conf_base64, 0, sizeof(t_conf_base64));
	base64_init(conf_base64);
	return (argp_parse(&base64_argp, argc, argv, ARGP_NO_EXIT | ARGP_NO_HELP, 0, conf_base64));
}