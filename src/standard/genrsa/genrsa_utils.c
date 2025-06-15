#include "commons.h"

void genrsa_init(t_conf_genrsa *conf_genrsa)
{
	conf_genrsa->output_fd = -1;
}

int genrsa_recoverable_error(t_conf *conf)
{
	t_conf_genrsa *genrsa = (t_conf_genrsa *)conf;

	(void)genrsa;
	return (0);
}
