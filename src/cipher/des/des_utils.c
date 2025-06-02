#include "commons.h"

void	des_init(t_conf_des *conf_des)
{
	conf_des->input_fd = -1;
	conf_des->output_fd = -1;
}

int	des_recoverable_error(t_conf *conf)
{
	t_conf_des	*des = (t_conf_des *)conf;

	(void)des;
	return (0);
}