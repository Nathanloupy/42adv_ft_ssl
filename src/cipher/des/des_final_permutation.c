#include "commons.h"

static const u_int8_t IP_INV[64] = {
	40, 8, 48, 16, 56, 24, 64, 32,
	39, 7, 47, 15, 55, 23, 63, 31,
	38, 6, 46, 14, 54, 22, 62, 30,
	37, 5, 45, 13, 53, 21, 61, 29,
	36, 4, 44, 12, 52, 20, 60, 28,
	35, 3, 43, 11, 51, 19, 59, 27,
	34, 2, 42, 10, 50, 18, 58, 26,
	33, 1, 41, 9, 49, 17, 57, 25
};

static u_int64_t	des_left_right_swap(u_int64_t input)
{
	u_int32_t	left_input;
	u_int32_t	right_input;

	left_input = (input >> 32) & 0xffffffff;
	right_input = input & 0xffffffff;
	return ((u_int64_t)right_input << 32) | left_input;
}

void	des_final_permutation(t_des *des)
{
	u_int64_t	before_permutation;
	u_int64_t	output;

	before_permutation = des_left_right_swap(des->final_round_output);
	output = 0;
	for (size_t i = 0; i < 64; i++)
	{
		if (before_permutation & (1ULL << (64 - IP_INV[i])))
			output |= (1ULL << (63 - i));
	}
	des->result = output;
}