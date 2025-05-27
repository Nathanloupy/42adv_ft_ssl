#include "commons.h"

static struct argp des_argp = {des_options, des_parse_opt, "", "DES encrypt or decrypt FILE, or standard input, to standard output.", NULL, NULL, NULL};

//TODO: check for support or not of multiple input/output files

error_t	des_parse_opt(int key, char *arg, struct argp_state *state)
{
	t_conf_des	*conf_des = state->input;

	switch (key)
	{
		case 'a':
			conf_des->flags |= FLAG_DES_BASE64;
			break;
		case 'd':
			conf_des->flags |= FLAG_DES_DECRYPT;
			break;
		case 'e':
			conf_des->flags |= FLAG_DES_ENCRYPT;
			break;
		case 'i':
			conf_des->flags |= FLAG_DES_INPUT_FILE;
			conf_des->input_file = arg;
			break;
		case 'o':
			conf_des->flags |= FLAG_DES_OUTPUT_FILE;
			conf_des->output_file = arg;
			break;
		case 'k':
			conf_des->flags |= FLAG_DES_KEY;
			conf_des->key = arg;
			break;
		case 's':
			conf_des->flags |= FLAG_DES_SALT;
			conf_des->salt = arg;
			break;
		case 'p':
			conf_des->flags |= FLAG_DES_PASSPHRASE;
			conf_des->passphrase = arg;
			break;
		case 'v':
			conf_des->flags |= FLAG_DES_IV;
			conf_des->iv = arg;
			break;
		case DES_OPTION_HELP:
			argp_help(&des_argp, stderr, ARGP_HELP_STD_HELP, state->argv[0]);
			conf_des->flags |= FLAG_DES_HELP;
			return (0);
		case DES_OPTION_USAGE:
			argp_help(&des_argp, stderr, ARGP_HELP_USAGE, state->argv[0]);
			conf_des->flags |= FLAG_DES_USAGE;
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

int	des_parser(int argc, char **argv, t_conf *conf)
{
	t_conf_des	*conf_des = (t_conf_des *)conf;
	memset(conf_des, 0, sizeof(t_conf_des));
	des_init(conf_des);
	des_set_mode(conf_des, argv[0]);
	return (argp_parse(&des_argp, argc, argv, ARGP_NO_EXIT | ARGP_NO_HELP, 0, conf_des));
}
