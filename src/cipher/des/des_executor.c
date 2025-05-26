#include "commons.h"

static void	des_data_init(t_des *des)
{
	memset(des, 0, sizeof(t_des));
}

int	des_executor(t_conf *conf)
{
	t_conf_des	*des = (t_conf_des *)conf;
	t_des		des_data;

	(void)des;
	des_data_init(&des_data);
	des_data.main_key = 0x6162636465666768;
	des_data.block = 0x3031323334353637;
	des_round_keys_generation(&des_data);
	des_initial_permutation(&des_data);
	des_rounds(&des_data);
	des_final_permutation(&des_data);
	printf("Result ciphered: %08lx\n", des_data.result);
	u_int64_t	temp = des_data.result;
	des_data_init(&des_data);
	des_data.main_key = 0x6162636465666768;
	des_data.block = temp;
	des_round_keys_generation(&des_data);
	des_reverse_round_keys(&des_data);
	des_initial_permutation(&des_data);
	des_rounds(&des_data);
	des_final_permutation(&des_data);
	printf("Result deciphered: %08lx\n", des_data.result);
	return (0);
}