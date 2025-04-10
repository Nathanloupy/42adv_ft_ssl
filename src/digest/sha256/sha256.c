#include "commons.h"

static void	fill_words(u_int32_t *words, const unsigned char *block)
{
	for (u_int32_t i = 0; i < 16; i++) //sha256 -> big endian
		words[i] = (block[i * 4] << 24) | (block[i * 4 + 1] << 16) | (block[i * 4 + 2] << 8) | block[i * 4 + 3];
}

static void	extend_words(u_int32_t *words)
{
	u_int32_t	s0 = 0;
	u_int32_t	s1 = 0;

	for (u_int32_t i = 16; i < 64; i++)
	{
		s0 = (ROTR(words[i - 15], 7) ^ ROTR(words[i - 15], 18) ^ (words[i - 15] >> 3));
		s1 = (ROTR(words[i - 2], 17) ^ ROTR(words[i - 2], 19) ^ (words[i - 2] >> 10));
		words[i] = words[i - 16] + s0 + words[i - 7] + s1;
	}
}

static void	sha256_process_complete_block(t_sha256 *sha256, const unsigned char *block)
{
	u_int32_t	words[64];
	u_int32_t	a = sha256->hash[0];
	u_int32_t	b = sha256->hash[1];
	u_int32_t	c = sha256->hash[2];
	u_int32_t	d = sha256->hash[3];
	u_int32_t	e = sha256->hash[4];
	u_int32_t	f = sha256->hash[5];
	u_int32_t	g = sha256->hash[6];
	u_int32_t	h = sha256->hash[7];

	fill_words(words, block);
	extend_words(words);
	for (u_int32_t i = 0; i < 64; i++)
	{
		u_int32_t	S1 = ROTR(e, 6) ^ ROTR(e, 11) ^ ROTR(e, 25);
		u_int32_t	ch = (e & f) ^ ((~e) & g);
		u_int32_t	temp1 = h + S1 + ch + SHA256_K[i] + words[i];
		u_int32_t	S0 = ROTR(a, 2) ^ ROTR(a, 13) ^ ROTR(a, 22);
		u_int32_t	maj = (a & b) ^ (a & c) ^ (b & c);
		u_int32_t	temp2 = S0 + maj;
		h = g;
		g = f;
		f = e;
		e = d + temp1;
		d = c;
		c = b;
		b = a;
		a = temp1 + temp2;
	}
	sha256->hash[0] += a;
	sha256->hash[1] += b;
	sha256->hash[2] += c;
	sha256->hash[3] += d;
	sha256->hash[4] += e;
	sha256->hash[5] += f;
	sha256->hash[6] += g;
	sha256->hash[7] += h;
}

static void	sha256_process_last_block(t_sha256 *sha256, const unsigned char *block, size_t remaining)
{
	unsigned char	padded_block[SHA256_BLOCK_SIZE];

	memset(padded_block, 0, SHA256_BLOCK_SIZE);
	if (block) 
	{
		if (remaining + 1 > SHA256_BLOCK_SIZE - 8)
		{
			memcpy(padded_block, block, remaining);
			padded_block[remaining] = 0x80;
			sha256_process_complete_block(sha256, padded_block);
			memset(padded_block, 0, SHA256_BLOCK_SIZE);
		}
		else
		{
			memcpy(padded_block, block, remaining);
			padded_block[remaining] = 0x80;
		}
	}
	else
		padded_block[0] = 0x80;

	u_int64_t initial_size_bits = sha256->data_size * 8;
	for (int i = 0; i < 8; i++) //sha256 -> big endian
		padded_block[SHA256_BLOCK_SIZE - 8 + i] = (initial_size_bits >> ((7 - i) * 8)) & 0xFF;
	sha256_process_complete_block(sha256, padded_block);
}

static char	*sha256_get_hash_str(t_sha256 *sha256)
{
	char	*hash_str = NULL;

	hash_str = calloc(65, sizeof(char));
	if (!hash_str)
		return (perror_ptr());
	for (int i = 0; i < 8; i++) //sha256 -> big endian
		sprintf(hash_str + (i * 8), "%08x", sha256->hash[i]);
	return (hash_str);
}

char	*sha256_process(t_sha256 *sha256, unsigned char *buffer, size_t buffer_size, int process_last_block)
{
	unsigned char	*block;
	char			*hash_str = NULL;
	size_t			remaining;
	size_t			last_handled_block_index = 0;
	
	if (buffer_size == 0 && process_last_block)
	{
		sha256_process_last_block(sha256, NULL, 0);
		hash_str = sha256_get_hash_str(sha256);
		return (hash_str);
	}

	for (size_t i = 0; i < buffer_size; i += SHA256_BLOCK_SIZE)
	{
		block = buffer + i;
		remaining = buffer_size - i;
		last_handled_block_index = i;

		if (remaining >= SHA256_BLOCK_SIZE)
		{
			sha256->data_size += SHA256_BLOCK_SIZE;
			sha256_process_complete_block(sha256, block);
		}
		else if (process_last_block)
		{
			sha256->data_size += remaining;
			sha256_process_last_block(sha256, block, remaining);
			hash_str = sha256_get_hash_str(sha256);
			return (hash_str);
		}
		else
			return (NULL);
	}
	if (process_last_block)
	{
		remaining = buffer_size - last_handled_block_index;
		sha256->data_size += remaining;
		sha256_process_last_block(sha256, block + last_handled_block_index, remaining);
		hash_str = sha256_get_hash_str(sha256);
		return (hash_str);
	}
	return (NULL);
}

void	sha256_init(t_sha256 *sha256)
{
	memset(sha256, 0, sizeof(t_sha256));
	for (int i = 0; i < 8; i++)
		sha256->hash[i] = SHA256_VALUES_INIT[i];
}