#include "commons.h"

int	des_executor(t_conf *conf)
{
	t_conf_des	*des = (t_conf_des *)conf;
	t_des		des_data;

	(void)des;
	des_data.main_key = 0x6162636465666768;
	des_round_key_generation(&des_data);
	for (size_t i = 0; i < 16; i++)
		printf("Round key %zu: %06lx\n", i, des_data.round_keys[i]);
	return (0);
}