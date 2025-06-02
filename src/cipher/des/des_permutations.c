#include "commons.h"

static const u_int8_t IP[64] = {
	58, 50, 42, 34, 26, 18, 10, 2,
	60, 52, 44, 36, 28, 20, 12, 4,
	62, 54, 46, 38, 30, 22, 14, 6,
	64, 56, 48, 40, 32, 24, 16, 8,
	57, 49, 41, 33, 25, 17, 9, 1,
	59, 51, 43, 35, 27, 19, 11, 3,
	61, 53, 45, 37, 29, 21, 13, 5,
	63, 55, 47, 39, 31, 23, 15, 7
};

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

void	des_initial_permutation(t_des *des)
{
	des->block_permuted = 0;
	for (size_t i = 0; i < 64; i++)
	{
		if (des->block & (1ULL << (64 - IP[i])))
			des->block_permuted |= (1ULL << (63 - i));
	}
}

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