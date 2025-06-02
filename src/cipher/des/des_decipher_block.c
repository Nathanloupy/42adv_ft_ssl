#include "commons.h"

u_int64_t	des_decipher_block(u_int64_t block, u_int64_t key)
{
	t_des	des;

	des.main_key = key;
	des.block = block;
	des_round_keys_generation(&des);
	des_reverse_round_keys(&des);
	des_initial_permutation(&des);
	des_rounds(&des);
	des_final_permutation(&des);
	return (des.result);
}

u_int64_t	des3_decipher_block(u_int64_t block, u_int64_t keys[3])
{
	u_int64_t	result;

	result = des_decipher_block(block, keys[2]);
	result = des_cipher_block(result, keys[1]);
	result = des_decipher_block(result, keys[0]);
	return (result);
}