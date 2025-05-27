#include "commons.h"

u_int64_t	des_cipher_block(u_int64_t block, u_int64_t key)
{
	t_des	des_data;

	memset(&des_data, 0, sizeof(t_des));
	des_data.block = block;
	des_data.main_key = key;
	des_round_keys_generation(&des_data);
	des_initial_permutation(&des_data);
	des_rounds(&des_data);
	des_final_permutation(&des_data);
	return (des_data.result);
}