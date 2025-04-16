#include "commons.h"

static const u_int8_t g_SBOX[16][16] = {
	{0x18, 0x23, 0xC6, 0xE8, 0x87, 0xB8, 0x01, 0x4F, 0x36, 0xA6, 0xD2, 0xF5, 0x79, 0x6F, 0x91, 0x52},
	{0x60, 0xBC, 0x9B, 0x8E, 0xA3, 0x0C, 0x7B, 0x35, 0x1D, 0xE0, 0xD7, 0xC2, 0x2E, 0x4B, 0xFE, 0x57},
	{0x15, 0x77, 0x37, 0xE5, 0x9F, 0xF0, 0x4A, 0xDA, 0x58, 0xC9, 0x29, 0x0A, 0xB1, 0xA0, 0x6B, 0x85},
	{0xBD, 0x5D, 0x10, 0xF4, 0xCB, 0x3E, 0x05, 0x67, 0xE4, 0x27, 0x41, 0x8B, 0xA7, 0x7D, 0x95, 0xD8},
	{0xFB, 0xEE, 0x7C, 0x66, 0xDD, 0x17, 0x47, 0x9E, 0xCA, 0x2D, 0xBF, 0x07, 0xAD, 0x5A, 0x83, 0x33},
	{0x63, 0x02, 0xAA, 0x71, 0xC8, 0x19, 0x49, 0xD9, 0xF2, 0xE3, 0x5B, 0x88, 0x9A, 0x26, 0x32, 0xB0},
	{0xE9, 0x0F, 0xD5, 0x80, 0xBE, 0xCD, 0x34, 0x48, 0xFF, 0x7A, 0x90, 0x5F, 0x20, 0x68, 0x1A, 0xAE},
	{0xB4, 0x54, 0x93, 0x22, 0x64, 0xF1, 0x73, 0x12, 0x40, 0x08, 0xC3, 0xEC, 0xDB, 0xA1, 0x8D, 0x3D},
	{0x97, 0x00, 0xCF, 0x2B, 0x76, 0x82, 0xD6, 0x1B, 0xB5, 0xAF, 0x6A, 0x50, 0x45, 0xF3, 0x30, 0xEF},
	{0x3F, 0x55, 0xA2, 0xEA, 0x65, 0xBA, 0x2F, 0xC0, 0xDE, 0x1C, 0xFD, 0x4D, 0x92, 0x75, 0x06, 0x8A},
	{0xB2, 0xE6, 0x0E, 0x1F, 0x62, 0xD4, 0xA8, 0x96, 0xF9, 0xC5, 0x25, 0x59, 0x84, 0x72, 0x39, 0x4C},
	{0x5E, 0x78, 0x38, 0x8C, 0xD1, 0xA5, 0xE2, 0x61, 0xB3, 0x21, 0x9C, 0x1E, 0x43, 0xC7, 0xFC, 0x04},
	{0x51, 0x99, 0x6D, 0x0D, 0xFA, 0xDF, 0x7E, 0x24, 0x3B, 0xAB, 0xCE, 0x11, 0x8F, 0x4E, 0xB7, 0xEB},
	{0x3C, 0x81, 0x94, 0xF7, 0xB9, 0x13, 0x2C, 0xD3, 0xE7, 0x6E, 0xC4, 0x03, 0x56, 0x44, 0x7F, 0xA9},
	{0x2A, 0xBB, 0xC1, 0x53, 0xDC, 0x0B, 0x9D, 0x6C, 0x31, 0x74, 0xF6, 0x46, 0xAC, 0x89, 0x14, 0xE1},
	{0x16, 0x3A, 0x69, 0x09, 0x70, 0xB6, 0xD0, 0xED, 0xCC, 0x42, 0x98, 0xA4, 0x28, 0x5C, 0xF8, 0x86}
};

static const u_int8_t g_C_MATRIX[8][8] = {
	{0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09},
	{0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05, 0x02},
	{0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08, 0x05},
	{0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01, 0x08},
	{0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04, 0x01},
	{0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01, 0x04},
	{0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01, 0x01},
	{0x01, 0x04, 0x01, 0x08, 0x05, 0x02, 0x09, 0x01}
};

void print_matrix_hex(u_int8_t (*matrix)[8][8])
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			printf("%02x ", (*matrix)[i][j]);
		printf("\n");
	}
	printf("\n");
}

void print_block_hex(const unsigned char *block)
{
	for (int i = 0; i < WHIRLPOOL_BLOCK_SIZE; i++)
	{
		printf("%02x ", block[i]);
		if ((i + 1) % 8 == 0)
			printf("\n");
	}
	printf("\n");
}

static void convert_block_to_matrix(u_int8_t (*matrix)[8][8], const unsigned char *block)
{
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
			(*matrix)[i][j] = block[i * 8 + j];
	}
}

static void assign_matrix_to_matrix(u_int8_t (*matrix_dest)[8][8], const u_int8_t (*matrix_src)[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			(*matrix_dest)[i][j] = (*matrix_src)[i][j];
}

static void sub_bytes(u_int8_t (*matrix)[8][8])
{
	u_int8_t row = 0;
	u_int8_t col = 0;

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			row = (*matrix)[i][j] >> 4 & 0x0F;
			col = (*matrix)[i][j] & 0x0F;
			(*matrix)[i][j] = g_SBOX[row][col];
		}
	}
}

static void shift_columns(u_int8_t (*matrix)[8][8])
{
	u_int8_t result[8][8];

	memset(result, 0, sizeof(result));
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			result[i][j] = (*matrix)[(i - j + 8) % 8][j];
	assign_matrix_to_matrix(matrix, &result);
}

static u_int8_t gf_multiply(u_int8_t a, u_int8_t b)
{
	u_int8_t p = 0;
	u_int8_t high_bit;
	
	for (int i = 0; i < 8; i++)
	{
		if (b & 1)
			p ^= a;
		high_bit = (a & 0x80);
		a <<= 1;
		if (high_bit)
			a ^= 0x1D;
		b >>= 1;
	}
	return (p);
}

static void mix_rows(u_int8_t (*matrix)[8][8])
{
	u_int8_t result[8][8];

	memset(result, 0, sizeof(result));
	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 8; j++)
		{
			for (int k = 0; k < 8; k++)
				result[i][j] ^= gf_multiply((*matrix)[i][k], g_C_MATRIX[k][j]);
		}
	}
	assign_matrix_to_matrix(matrix, &result);
}

static void add_key(u_int8_t (*matrix_dest)[8][8], const u_int8_t (*matrix_src)[8][8])
{
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			(*matrix_dest)[i][j] ^= (*matrix_src)[i][j];
}

static void add_rc(u_int8_t (*key)[8][8], int r)
{
	u_int8_t index = 0;
	u_int8_t row = 0;
	u_int8_t col = 0;

	if (r == 0)
		return;
	for (int j = 0; j < 8; j++)
	{
		index = 8 * (r - 1) + j;
		row = index >> 4 & 0x0F;
		col = index & 0x0F;
		(*key)[0][j] ^= g_SBOX[row][col];
	}
}

static void expand_key(u_int8_t (*key)[8][8], int r)
{
	sub_bytes(key);
	shift_columns(key);
	mix_rows(key);
	add_rc(key, r);
}

static void whirlpool_process_complete_block(t_whirlpool *whirlpool, const unsigned char *block)
{
	u_int8_t message[8][8];
	u_int8_t key[8][8];
	u_int8_t state[8][8];


	memset(message, 0, sizeof(message));
	memset(key, 0, sizeof(key));

	convert_block_to_matrix(&message, block);
	assign_matrix_to_matrix(&state, &message);
	assign_matrix_to_matrix(&key, &whirlpool->hash_matrix);

	add_key(&state, &key);

	for (int r = 1; r < 11; r++)
	{
		sub_bytes(&state);
		shift_columns(&state);
		mix_rows(&state);
		expand_key(&key, r);
		add_key(&state, &key);
	}

	u_int8_t result[8][8];
	for (int i = 0; i < 8; i++)
		for (int j = 0; j < 8; j++)
			result[i][j] = state[i][j] ^ whirlpool->hash_matrix[i][j] ^ message[i][j];
	assign_matrix_to_matrix(&whirlpool->hash_matrix, &result);
}

static void whirlpool_process_last_block(t_whirlpool *whirlpool, const unsigned char *block, size_t remaining)
{
	unsigned char padded_block[WHIRLPOOL_BLOCK_SIZE];

	memset(padded_block, 0, WHIRLPOOL_BLOCK_SIZE);
	
	if (block) 
	{
		if (remaining + 1 > WHIRLPOOL_BLOCK_SIZE - 32) /* 32 bytes = 256 bits for length */
		{
			memcpy(padded_block, block, remaining);
			padded_block[remaining] = 0x80;
			whirlpool_process_complete_block(whirlpool, padded_block);
			memset(padded_block, 0, WHIRLPOOL_BLOCK_SIZE);
		}
		else
		{
			memcpy(padded_block, block, remaining);
			padded_block[remaining] = 0x80;
		}
	}
	else
		padded_block[0] = 0x80;
	
	u_int64_t initial_size_bits = whirlpool->data_size * 8;
	for (int i = 0; i < 32; i++)
	{
		if (i < 24)
			padded_block[WHIRLPOOL_BLOCK_SIZE - 32 + i] = 0;
		else
			padded_block[WHIRLPOOL_BLOCK_SIZE - 32 + i] = (initial_size_bits >> ((31 - i) * 8)) & 0xFF;
	}	
	whirlpool_process_complete_block(whirlpool, padded_block);
}

static char *whirlpool_get_hash_str(t_whirlpool *whirlpool)
{
	char *hash_str = NULL;

	hash_str = calloc(129, sizeof(char));
	if (!hash_str)
		return (NULL);
	for (int i = 0; i < 64; i++)
		sprintf(hash_str + i * 2, "%02x", whirlpool->hash_matrix[i / 8][i % 8]);
	return (hash_str);
}

char	*whirlpool_process(t_whirlpool *whirlpool, unsigned char *buffer, size_t buffer_size, int process_last_block)
{
	unsigned char	*block;
	char			*hash_str = NULL;
	size_t			remaining;
	size_t			last_handled_block_index = 0;
	
	if (buffer_size == 0 && process_last_block)
	{
		whirlpool_process_last_block(whirlpool, NULL, 0);
		hash_str = whirlpool_get_hash_str(whirlpool);
		return (hash_str);
	}

	for (size_t i = 0; i < buffer_size; i += WHIRLPOOL_BLOCK_SIZE)
	{
		block = buffer + i;
		remaining = buffer_size - i;
		last_handled_block_index = i;

		if (remaining >= WHIRLPOOL_BLOCK_SIZE)
		{
			whirlpool->data_size += WHIRLPOOL_BLOCK_SIZE;
			whirlpool_process_complete_block(whirlpool, block);
		}
		else if (process_last_block)
		{
			whirlpool->data_size += remaining;
			whirlpool_process_last_block(whirlpool, block, remaining);
			hash_str = whirlpool_get_hash_str(whirlpool);
			return (hash_str);
		}
		else
			return (NULL);
	}
	if (process_last_block)
	{
		remaining = buffer_size - last_handled_block_index;
		whirlpool->data_size += remaining;
		whirlpool_process_last_block(whirlpool, block + last_handled_block_index, remaining);
		hash_str = whirlpool_get_hash_str(whirlpool);
		return (hash_str);
	}
	return (NULL);
}

void whirlpool_init(t_whirlpool *whirlpool)
{
	memset(whirlpool, 0, sizeof(t_whirlpool));
}
