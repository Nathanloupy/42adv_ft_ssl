#include "commons.h"

void rsa_init(t_conf_rsa *conf_rsa)
{
	conf_rsa->output_fd = -1;
	conf_rsa->input_file = NULL;
	conf_rsa->output_file = NULL;
	conf_rsa->passin = NULL;
	conf_rsa->passout = NULL;
	conf_rsa->inform = RSA_FORMAT_PEM;
	conf_rsa->outform = RSA_FORMAT_PEM;
}

int rsa_recoverable_error(t_conf *conf)
{
	t_conf_rsa *conf_rsa = (t_conf_rsa *)conf;

	(void)conf_rsa;
	return (0);
}
