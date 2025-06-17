#include "commons.h"

static struct argp rsa_argp = {rsa_options, rsa_parse_opt, NULL, "RSA key processing tool", NULL, NULL, NULL};

static int parse_format(char *format_str, enum e_rsa_format *format)
{
	if (!format_str)
	{
		*format = RSA_FORMAT_PEM;
		return (0);
	}
	if (!strcmp(format_str, "PEM"))
		*format = RSA_FORMAT_PEM;
	else if (!strcmp(format_str, "DER"))
		*format = RSA_FORMAT_DER;
	else if (!strcmp(format_str, "NET"))
		*format = RSA_FORMAT_NET;
	else
		return (-1); // Unknown format
	return (0);
}

static void handle_format_error(char *format_str, const char *format_type, struct argp_state *state)
{
	argp_error(state, "Invalid %s format: %s. Valid formats are: PEM, DER, NET", format_type, format_str);
}

error_t rsa_parse_opt(int key, char *arg, struct argp_state *state)
{
	t_conf_rsa *conf = state->input;

	switch (key)
	{
		case 'i':
			if (conf->flags & FLAG_RSA_INPUT_FILE)
				return (ARGP_ERR_UNKNOWN);
			conf->flags |= FLAG_RSA_INPUT_FILE;
			conf->input_file = arg;
			break;
		case 'o':
			if (conf->flags & FLAG_RSA_OUTPUT_FILE)
				return (ARGP_ERR_UNKNOWN);
			conf->flags |= FLAG_RSA_OUTPUT_FILE;
			conf->output_file = arg;
			break;
		case RSA_OPTION_INFORM:
			conf->flags |= FLAG_RSA_INFORM;
			if (parse_format(arg, &conf->inform) != 0)
				handle_format_error(arg, "input", state);
			break;
		case RSA_OPTION_OUTFORM:
			conf->flags |= FLAG_RSA_OUTFORM;
			if (parse_format(arg, &conf->outform) != 0)
				handle_format_error(arg, "output", state);
			break;
		case RSA_OPTION_PASSIN:
			conf->flags |= FLAG_RSA_PASSIN;
			conf->passin = arg;
			break;
		case RSA_OPTION_PASSOUT:
			conf->flags |= FLAG_RSA_PASSOUT;
			conf->passout = arg;
			break;
		case RSA_OPTION_DES:
			conf->flags |= FLAG_RSA_DES;
			break;
		case RSA_OPTION_TEXT:
			conf->flags |= FLAG_RSA_TEXT;
			break;
		case RSA_OPTION_NOOUT:
			conf->flags |= FLAG_RSA_NOOUT;
			break;
		case RSA_OPTION_MODULUS:
			conf->flags |= FLAG_RSA_MODULUS;
			break;
		case RSA_OPTION_CHECK:
			conf->flags |= FLAG_RSA_CHECK;
			break;
		case RSA_OPTION_PUBIN:
			conf->flags |= FLAG_RSA_PUBIN;
			break;
		case RSA_OPTION_PUBOUT:
			conf->flags |= FLAG_RSA_PUBOUT;
			break;
		case RSA_OPTION_HELP:
			argp_help(&rsa_argp, stderr, ARGP_HELP_STD_HELP, state->argv[0]);
			conf->flags |= FLAG_RSA_HELP;
			return (0);
		case RSA_OPTION_USAGE:
			argp_help(&rsa_argp, stderr, ARGP_HELP_USAGE, state->argv[0]);
			conf->flags |= FLAG_RSA_USAGE;
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

int rsa_parser(int argc, char **argv, t_conf *conf)
{
	t_conf_rsa *conf_rsa = (t_conf_rsa *)conf;
	memset(conf_rsa, 0, sizeof(t_conf_rsa));
	rsa_init(conf_rsa);
	return (argp_parse(&rsa_argp, argc, argv, ARGP_NO_EXIT | ARGP_NO_HELP, 0, conf_rsa));
}
