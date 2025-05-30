#include "commons.h"

void	des_init(t_conf_des *conf_des)
{
	conf_des->input_fd = -1;
	conf_des->output_fd = -1;
}

void	des_set_mode(t_conf_des *conf_des, char *command)
{
	if (!strcmp(command, "des-ecb"))
		conf_des->mode = DES_ECB;
	else if (!strcmp(command, "des-cbc") || !strcmp(command, "des"))
		conf_des->mode = DES_CBC;
	else if (!strcmp(command, "des3-ecb"))
		conf_des->mode = DES3_ECB;
	else if (!strcmp(command, "des3-cbc") || !strcmp(command, "des3"))
		conf_des->mode = DES3_CBC;
	else
		conf_des->mode = DES_NULL;
}

int	des_recoverable_error(t_conf *conf)
{
	t_conf_des	*des = (t_conf_des *)conf;

	(void)des;
	return (0);
}