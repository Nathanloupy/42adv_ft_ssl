#include "commons.h"

static const u_int8_t PC1[56] = {
	57, 49, 41, 33, 25, 17, 9,
	1, 58, 50, 42, 34, 26, 18,
	10, 2, 59, 51, 43, 35, 27,
	19, 11, 3, 60, 52, 44, 36,
	63, 55, 47, 39, 31, 23, 15,
	7, 62, 54, 46, 38, 30, 22,
	14, 6, 61, 53, 45, 37, 29,
	21, 13, 5, 28, 20, 12, 4
};

static const u_int8_t SHIFT[16] = {
	1, 1, 2, 2, 2, 2, 2, 2,
	1, 2, 2, 2, 2, 2, 2, 1
};

static const u_int8_t PC2[48] = {
	14, 17, 11, 24, 1, 5,
	3, 28, 15, 6, 21, 10,
	23, 19, 12, 4, 26, 8,
	16, 7, 27, 20, 13, 2,
	41, 52, 31, 37, 47, 55,
	30, 40, 51, 45, 33, 48,
	44, 49, 39, 56, 34, 53,
	46, 42, 50, 36, 29, 32
};

static u_int32_t	rotate_left_28(u_int32_t value, u_int8_t shift)
{
    value &= 0x0fffffff;
    return ((value << shift) | (value >> (28 - shift))) & 0x0fffffff;
}

void	des_round_keys_generation(t_des *des)
{
	u_int32_t c = 0;		// 28 bits
	u_int32_t d = 0;		// 28 bits
	u_int64_t cd;			// 56 bits
	u_int32_t left_half;	// 24 bits
	u_int32_t right_half;	// 24 bits

	memset(des->round_keys, 0, sizeof(des->round_keys));
	
	for (size_t i = 0; i < 28; i++)
	{
		if (des->main_key & (1ULL << (64 - PC1[i])))
			c |= (1ULL << (27 - i));
		if (des->main_key & (1ULL << (64 - PC1[i + 28])))
			d |= (1ULL << (27 - i));
	}
	for (size_t i = 0; i < 16; i++)
	{
		c = rotate_left_28(c, SHIFT[i]);
		d = rotate_left_28(d, SHIFT[i]);
		left_half = 0;
		right_half = 0;
		cd = ((u_int64_t)c << 28) | d;
		for (size_t j = 0; j < 24; j++)
		{
			if (cd & (1ULL << (56 - PC2[j])))
				left_half |= (1ULL << (23 - j));
			if (cd & (1ULL << (56 - PC2[j + 24])))
				right_half |= (1ULL << (23 - j));
		}
		des->round_keys[i] = ((u_int64_t)left_half << 24) | right_half;
	}
}

void	des_reverse_round_keys(t_des *des)
{
	u_int64_t	temp;

	for (size_t i = 0; i < 8; i++)
	{
		temp = des->round_keys[i];
		des->round_keys[i] = des->round_keys[15 - i];
		des->round_keys[15 - i] = temp;
	}
}