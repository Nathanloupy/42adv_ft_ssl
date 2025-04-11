#include "commons.h"

static error_t parse_opt(int key, char *arg, struct argp_state *state)
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
	struct argp argp = {base64_options, parse_opt, "[FILE...]", "FILE... files to digest (default is stdin)", NULL, NULL, NULL};
	return (argp_parse(&argp, argc, argv, ARGP_NO_EXIT, 0, conf_base64));
}