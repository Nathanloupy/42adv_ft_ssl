#include "commons.h"

static void	md5_process_complete_block(t_md5 *md5, const unsigned char *block)
{
	static int block_count = 0;
	printf("Processing block %d : \n", block_count++);
	print_complete_block(block);
	//TODO: implement
	(void)md5;
}

static void	md5_process_last_block(t_md5 *md5, const unsigned char *block, size_t remaining)
{
	unsigned char	padded_block[MD5_BLOCK_SIZE];

	memset(padded_block, 0, MD5_BLOCK_SIZE);
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
	u_int64_t initial_size_bits = md5->data_size * 8;
	for (int i = 0; i < 8; i++)
		padded_block[MD5_BLOCK_SIZE - 8 + i] = (initial_size_bits >> (i * 8)) & 0xFF;
	md5_process_complete_block(md5, padded_block);
}

static char	*md5_get_hash_str(t_md5 *md5)
{
	char	*hash_str = NULL;
	(void)md5;
	return (hash_str);
}

char	*md5_process(t_md5 *md5, unsigned char *buffer, size_t buffer_size, int process_last_block)
{
	unsigned char	*block;
	char			*hash_str = NULL;
	size_t			remaining;

	if (!buffer)
		return (NULL);
	for (size_t i = 0; i < buffer_size; i += MD5_BLOCK_SIZE)
	{
		block = buffer + i;
		remaining = buffer_size - i;
		md5->data_size += remaining;
		
		if (remaining == MD5_BLOCK_SIZE)
			md5_process_complete_block(md5, block);
		else if (!process_last_block)
			return (NULL);
		else
		{
			md5_process_last_block(md5, block, remaining);
			hash_str = md5_get_hash_str(md5);
		}
	}
	return (hash_str);
}

void	md5_init(t_md5 *md5)
{
	memset(md5, 0, sizeof(t_md5));
	md5->hash[0] = MD5_A0;
	md5->hash[1] = MD5_B0;
	md5->hash[2] = MD5_C0;
	md5->hash[3] = MD5_D0;
}

int	md5_executor(t_conf *conf)
{
	t_conf_digest *conf_digest = (t_conf_digest *)conf;
	(void)conf_digest;

	t_md5 md5;
	md5_init(&md5);
	unsigned char *fifty_six = calloc(57, sizeof(unsigned char));
	memset(fifty_six, 'a', 56);
	fifty_six[56] = '\0';
	printf("fifty_six: %s\n", fifty_six);
	char *result = md5_process(&md5, fifty_six, 56, 1);
	if (result) {
		printf("MD5 hash: %s\n", result);
		free(result);
	}
	free(fifty_six);
	return (0);
}