#include "commons.h"

#define DES_PBKDF2_ITERATIONS 10000

static char	*simple_sha256(const char *data, size_t data_len)
{
	t_sha256		sha256;
	char			*hash_str;
	
	sha256_init(&sha256);
	hash_str = sha256_process(&sha256, (unsigned char *)data, data_len, 1);
	if (!hash_str)
		return (NULL);
	return (hash_str);
}

static int hmac_sha256(const unsigned char *key, size_t key_len, const unsigned char *data, size_t data_len, unsigned char *output)
{
	t_sha256		sha256;
	char			*hash_str;
	unsigned char	ipad[SHA256_BLOCK_SIZE];
	unsigned char	opad[SHA256_BLOCK_SIZE];
	unsigned char	key_pad[SHA256_BLOCK_SIZE];

	if (!key || !data || !output)
		return (-1);

	memset(key_pad, 0, SHA256_BLOCK_SIZE);
	if (key_len > SHA256_BLOCK_SIZE)
	{
		hash_str = simple_sha256(key, key_len);
		if (!hash_str)
			return (-1);
		for (size_t i = 0; i < SHA256_HASH_SIZE; i++)
		{
			char hex_byte[3] = {hash_str[i * 2], hash_str[i * 2 + 1], '\0'};
			key_pad[i] = (unsigned char)strtol(hex_byte, NULL, 16);
		}
		free(hash_str);
	}
	else
		memcpy(key_pad, key, key_len);

	for (size_t i = 0; i < SHA256_BLOCK_SIZE; i++)
	{
		ipad[i] = key_pad[i] ^ 0x36;
		opad[i] = key_pad[i] ^ 0x5C;
	}

	unsigned char *inner_input = calloc(SHA256_BLOCK_SIZE + data_len, sizeof(unsigned char));
	if (!inner_input)
		return (-1);
	memcpy(inner_input, ipad, SHA256_BLOCK_SIZE);
	memcpy(inner_input + SHA256_BLOCK_SIZE, data, data_len);
	
	hash_str = simple_sha256(inner_input, SHA256_BLOCK_SIZE + data_len);
	free(inner_input);
	if (!hash_str)
		return (-1);
	
	unsigned char inner_hash[SHA256_HASH_SIZE];
	for (size_t i = 0; i < SHA256_HASH_SIZE; i++)
	{
		char hex_byte[3] = {hash_str[i * 2], hash_str[i * 2 + 1], '\0'};
		inner_hash[i] = (unsigned char)strtol(hex_byte, NULL, 16);
	}
	free(hash_str);
	
	// Outer hash: H(K XOR opad || inner_hash)
	unsigned char outer_input[SHA256_BLOCK_SIZE + SHA256_HASH_SIZE];
	memcpy(outer_input, opad, SHA256_BLOCK_SIZE);
	memcpy(outer_input + SHA256_BLOCK_SIZE, inner_hash, SHA256_HASH_SIZE);
	
	hash_str = simple_sha256(outer_input, SHA256_BLOCK_SIZE + SHA256_HASH_SIZE);
	if (!hash_str)
		return (-1);
	
	for (size_t i = 0; i < SHA256_HASH_SIZE; i++)
	{
		char hex_byte[3] = {hash_str[i * 2], hash_str[i * 2 + 1], '\0'};
		output[i] = (unsigned char)strtol(hex_byte, NULL, 16);
	}
	free(hash_str);
	return (0);
}

static int pbkdf2_sha256(const char *passphrase, const char *salt, size_t passphrase_len, size_t salt_len, int iterations, unsigned char *output, size_t output_len)
{
	unsigned char	*salt_with_counter;
	unsigned char	block_result[SHA256_HASH_SIZE];
	unsigned char	temp_result[SHA256_HASH_SIZE];
	size_t			salt_counter_len;
	int				block_index = 1;
	size_t			bytes_generated = 0;
	
	if (!passphrase || !output || iterations <= 0 || output_len == 0)
		return (-1);
	
	salt_counter_len = salt_len + 4;
	salt_with_counter = calloc(salt_counter_len, sizeof(unsigned char));
	if (!salt_with_counter)
		return (-1);
	
	if (salt && salt_len > 0)
		memcpy(salt_with_counter, salt, salt_len);
	
	while (bytes_generated < output_len)
	{
		salt_with_counter[salt_len] = (block_index >> 24) & 0xFF;
		salt_with_counter[salt_len + 1] = (block_index >> 16) & 0xFF;
		salt_with_counter[salt_len + 2] = (block_index >> 8) & 0xFF;
		salt_with_counter[salt_len + 3] = block_index & 0xFF;
		
		if (hmac_sha256((unsigned char *)passphrase, passphrase_len, salt_with_counter, salt_counter_len, block_result) != 0)
		{
			free(salt_with_counter);
			return (-1);
		}
		
		memcpy(temp_result, block_result, SHA256_HASH_SIZE);
		
		for (size_t i = 1; i < (size_t)iterations; i++)
		{
			if (hmac_sha256((unsigned char *)passphrase, passphrase_len, temp_result, SHA256_HASH_SIZE, temp_result) != 0)
			{
				free(salt_with_counter);
				return (-1);
			}

			for (size_t j = 0; j < SHA256_HASH_SIZE; j++)
				block_result[j] ^= temp_result[j];
		}
		
		size_t copy_len = ((output_len - bytes_generated) < SHA256_HASH_SIZE) ? (output_len - bytes_generated) : SHA256_HASH_SIZE;
		memcpy(output + bytes_generated, block_result, copy_len);
		bytes_generated += copy_len;
		block_index++;
	}
	free(salt_with_counter);
	return (0);
}

int	des_derive_key(u_int64_t (*keys)[3], char *passphrase, char *salt, size_t number_of_keys)
{
	unsigned char	key_bytes[24];
	u_int64_t		key;
	
	if (!passphrase)

		return (1);

	if (pbkdf2_sha256(passphrase, salt, strlen(passphrase), salt ? strlen(salt) : 0, DES_PBKDF2_ITERATIONS, key_bytes, 8 * number_of_keys) != 0)
		return (1);

	for (size_t i = 0; i < number_of_keys; i++)
	{
		key = 0;
		for (size_t j = 0; j < 8; j++)
			key = (key << 8) | key_bytes[i * 8 + j];
		(*keys)[i] = key;
	}
	
	return (0);
}