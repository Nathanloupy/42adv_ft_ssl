#include "commons.h"

static void	fill_words(u_int32_t *words, const unsigned char *block)
{
	for (u_int32_t i = 0; i < 16; i++) //md5 -> little endian
		words[i] = block[i * 4] | (block[i * 4 + 1] << 8) | (block[i * 4 + 2] << 16) | (block[i * 4 + 3] << 24);
}

static void	md5_process_complete_block(t_md5 *md5, const unsigned char *block)
{
	u_int32_t	words[16];
	u_int32_t	a = md5->hash[0];
	u_int32_t	b = md5->hash[1];
	u_int32_t	c = md5->hash[2];
	u_int32_t	d = md5->hash[3];

	fill_words(words, block);
	for (u_int32_t i = 0; i < 64; i++)
	{
		u_int32_t	f = 0;
		u_int32_t	g = 0;

		if (i <= 15)
		{
			f = MD5_F(b, c, d);
			g = i;
		}
		else if (i >= 16 && i <= 31)
		{
			f = MD5_G(b, c, d);
			g = (5 * i + 1) % 16;
		}
		else if (i >= 32 && i <= 47)
		{
			f = MD5_H(b, c, d);
			g = (3 * i + 5) % 16;
		}
		else if (i >= 48)
		{
			f = MD5_I(b, c, d);
			g = (7 * i) % 16;
		}
		f += a + MD5_K[i] + words[g];
		a = d;
		d = c;
		c = b;
		b += ROTL(f, MD5_SHIFTS[i]);
	}
	md5->hash[0] += a;
	md5->hash[1] += b;
	md5->hash[2] += c;
	md5->hash[3] += d;
}

static void	md5_process_last_block(t_md5 *md5, const unsigned char *block, size_t remaining)
{
	unsigned char	padded_block[MD5_BLOCK_SIZE];

	memset(padded_block, 0, MD5_BLOCK_SIZE);
	if (block) 
	{
		if (remaining >= MD5_BLOCK_SIZE - 8)
		{
			memcpy(padded_block, block, remaining);
			padded_block[remaining] = 0x80;
			md5_process_complete_block(md5, padded_block);
			memset(padded_block, 0, MD5_BLOCK_SIZE);
		}
		else
		{
			memcpy(padded_block, block, remaining);
			padded_block[remaining] = 0x80;
		}
	}
	else
		padded_block[0] = 0x80;

	u_int64_t initial_size_bits = md5->data_size * 8;
	for (int i = 0; i < 8; i++) //md5 -> little endian
		padded_block[MD5_BLOCK_SIZE - 8 + i] = (initial_size_bits >> (i * 8)) & 0xFF;
	md5_process_complete_block(md5, padded_block);
}

static char	*md5_get_hash_str(t_md5 *md5)
{
	char	*hash_str = NULL;

	hash_str = calloc(33, sizeof(char));
	if (!hash_str)
		return (perror_ptr());
	for (int i = 0; i < 4; i++) //md5 -> little endian
	{
		for (int j = 0; j < 4; j++)
			sprintf(hash_str + (i * 8 + j * 2), "%02x", (md5->hash[i] >> (j * 8)) & 0xFF);
	}
	return (hash_str);
}

char	*md5_process(t_md5 *md5, unsigned char *buffer, size_t buffer_size, int process_last_block)
{
	unsigned char	*block;
	char			*hash_str = NULL;
	size_t			remaining;
	size_t			last_handled_block_index = 0;
	
	if (buffer_size == 0 && process_last_block)
	{
		md5_process_last_block(md5, NULL, 0);
		hash_str = md5_get_hash_str(md5);
		return (hash_str);
	}

	for (size_t i = 0; i < buffer_size; i += MD5_BLOCK_SIZE)
	{
		block = buffer + i;
		remaining = buffer_size - i;
		last_handled_block_index = i;

		if (remaining >= MD5_BLOCK_SIZE)
		{
			md5->data_size += MD5_BLOCK_SIZE;
			md5_process_complete_block(md5, block);
		}
		else if (process_last_block)
		{
			md5->data_size += remaining;
			md5_process_last_block(md5, block, remaining);
			hash_str = md5_get_hash_str(md5);
			return (hash_str);
		}
		else
			return (NULL);
	}
	if (process_last_block)
	{
		remaining = buffer_size - last_handled_block_index;
		md5->data_size += remaining;
		md5_process_last_block(md5, block + last_handled_block_index, remaining);
		hash_str = md5_get_hash_str(md5);
		return (hash_str);
	}
	return (NULL);
}

void	md5_init(t_md5 *md5)
{
	memset(md5, 0, sizeof(t_md5));
	md5->hash[0] = MD5_A0;
	md5->hash[1] = MD5_B0;
	md5->hash[2] = MD5_C0;
	md5->hash[3] = MD5_D0;
}