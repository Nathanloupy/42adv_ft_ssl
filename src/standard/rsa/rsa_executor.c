#include "commons.h"

int rsa_executor(t_conf *conf)
{
	t_conf_rsa	*conf_rsa = (t_conf_rsa *)conf;

	if (conf_rsa->flags & FLAG_RSA_HELP || conf_rsa->flags & FLAG_RSA_USAGE)
		return (0);

	(void)conf_rsa;
	return (0);
}
