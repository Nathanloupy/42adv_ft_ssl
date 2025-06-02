#include "commons.h"

int	des_execute_cipher(t_exec_des *exec_des)
{
	size_t			padded_size;
	u_int64_t		block;
	u_int64_t		ciphered_block;
	u_int64_t		prev_ciphertext;
	char			*padded_input;
	unsigned char	padding_value;

	padding_value = 8 - (exec_des->input_buffer_size % 8);
	padded_size = exec_des->input_buffer_size + padding_value;
	
	padded_input = calloc(padded_size, sizeof(char));
	if (!padded_input)
		return (perror_int());
	memcpy(padded_input, exec_des->input_buffer, exec_des->input_buffer_size);
	
	for (size_t i = exec_des->input_buffer_size; i < padded_size; i++)
		padded_input[i] = padding_value;
	
	exec_des->output_buffer = calloc(padded_size, sizeof(char));
	if (!exec_des->output_buffer)
		return (free(padded_input), perror_int());
	exec_des->output_buffer_size = padded_size;
	
	prev_ciphertext = exec_des->iv;
	for (size_t i = 0; i < padded_size / 8; i++)
	{
		block = 0;
		for (size_t j = 0; j < 8; j++)
			block |= ((u_int64_t)(unsigned char)padded_input[i * 8 + j]) << (56 - j * 8);
		
		if (exec_des->mode & DES_CBC)
			block ^= prev_ciphertext;
		
		if (exec_des->mode & DES_TRIPLE)
			ciphered_block = des3_cipher_block(block, exec_des->keys);
		else
			ciphered_block = des_cipher_block(block, exec_des->keys[0]);
		
		for (size_t j = 0; j < 8; j++)
			exec_des->output_buffer[i * 8 + j] = (ciphered_block >> (56 - j * 8)) & 0xFF;
		
		prev_ciphertext = ciphered_block;
	}
	free(padded_input);
	return (0);
}

int	des_execute_decipher(t_exec_des *exec_des)
{
	size_t			input_size;
	u_int64_t		block;
	u_int64_t		deciphered_block;
	u_int64_t		prev_ciphertext;
	u_int64_t		current_ciphertext;
	unsigned char	padding_value;
	size_t			actual_size;

	input_size = exec_des->input_buffer_size;
	
	if (input_size % 8 != 0)
		return (fprintf(stderr, "%s: invalid input\n", FT_SSL_NAME), 1);
	
	exec_des->output_buffer = calloc(input_size, sizeof(char));
	if (!exec_des->output_buffer)
		return (perror_int());
	exec_des->output_buffer_size = input_size;
	
	prev_ciphertext = exec_des->iv;
	for (size_t i = 0; i < input_size / 8; i++)
	{
		block = 0;
		for (size_t j = 0; j < 8; j++)
			block |= ((u_int64_t)(unsigned char)exec_des->input_buffer[i * 8 + j]) << (56 - j * 8);
		
		current_ciphertext = block;
		if (exec_des->mode & DES_TRIPLE)
			deciphered_block = des3_decipher_block(block, exec_des->keys);
		else
			deciphered_block = des_decipher_block(block, exec_des->keys[0]);
		
		if (exec_des->mode & DES_CBC)
			deciphered_block ^= prev_ciphertext;
		
		for (size_t j = 0; j < 8; j++)
			exec_des->output_buffer[i * 8 + j] = (deciphered_block >> (56 - j * 8)) & 0xFF;

		prev_ciphertext = current_ciphertext;
	}
	
	if (input_size > 0)
	{
		padding_value = (unsigned char)exec_des->output_buffer[input_size - 1];
		if (padding_value == 0 || padding_value > 8)
			return (fprintf(stderr, "%s: invalid input\n", FT_SSL_NAME), 1);
		for (size_t i = input_size - padding_value; i < input_size; i++)
		{
			if ((unsigned char)exec_des->output_buffer[i] != padding_value)
				return (fprintf(stderr, "%s: invalid input\n", FT_SSL_NAME), 1);
		}
		actual_size = input_size - padding_value;
		exec_des->output_buffer_size = actual_size;
	}
	return (0);
}