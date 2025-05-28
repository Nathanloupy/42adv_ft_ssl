#include "commons.h"

void	des_string_length_error(size_t size)
{
	if (size < 16)
		fprintf(stderr, "%s: hex string is too short, padding with zero bytes to length 16\n", FT_SSL_NAME);
	else if (size > 16)
		fprintf(stderr, "%s: hex string is too long, ignoring excess\n", FT_SSL_NAME);
}

static void	des_free_exec(t_exec_des *exec_des)
{
	if (exec_des->input_buffer)
		free(exec_des->input_buffer);
	if (exec_des->output_buffer)
		free(exec_des->output_buffer);
	memset(exec_des, 0, sizeof(t_exec_des));
}

static int	add_to_input_buffer(char *buffer, size_t size, size_t *input_size, char **input)
{
	char	*temp;

	temp = calloc(*input_size + size, sizeof(char));
	if (!temp)
		return (1);
	memcpy(temp, *input, *input_size);
	memcpy(temp + *input_size, buffer, size);
	*input_size += size;
	free(*input);
	*input = temp;
	return (0);
}

static int	des_execute_cipher(t_exec_des *exec_des)
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
		
		if (exec_des->mode == DES_CBC)
			block ^= prev_ciphertext;
		
		ciphered_block = des_cipher_block(block, exec_des->key);
		
		for (size_t j = 0; j < 8; j++)
			exec_des->output_buffer[i * 8 + j] = (ciphered_block >> (56 - j * 8)) & 0xFF;
		
		prev_ciphertext = ciphered_block;
	}
	free(padded_input);
	return (0);
}

static int	des_execute_decipher(t_exec_des *exec_des)
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
		deciphered_block = des_decipher_block(block, exec_des->key);
		
		if (exec_des->mode == DES_CBC)
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

int	des_executor(t_conf *conf)
{
	t_conf_des	*conf_des = (t_conf_des *)conf;
	t_exec_des	exec_des;
	char		temp_str[17];
	char		*temp_buffer;
	ssize_t		bytes_read;

	memset(&exec_des, 0, sizeof(t_exec_des));
	exec_des.mode = conf_des->mode;
	if (conf_des->flags & FLAG_DES_KEY)
	{
		//TODO: check strings length and composition
		memset(temp_str, 0, sizeof(temp_str));
		strncpy(temp_str, conf_des->key, 16);
		exec_des.key = strtoull(temp_str, NULL, 16);
	}
	else
	{
		if (conf_des->flags & FLAG_DES_PASSPHRASE)
		{
			//TODO: generate key from passphrase and salt (truncate to 8 bytes)
		}
		else
		{
			//TODO: get the passphrase from stdin (truncate to 8 bytes)
		}
		//TODO: generate key from passphrase
	}
	if (conf_des->flags & FLAG_DES_IV)
	{
		//TODO: check strings length and composition
		memset(temp_str, 0, sizeof(temp_str));
		strncpy(temp_str, conf_des->iv, 16);
		exec_des.iv = strtoull(temp_str, NULL, 16);
	}
	exec_des.key = 0x6162636465666768;
	exec_des.iv = 0x3031323334353637;
	if (conf_des->flags & FLAG_DES_INPUT_FILE)
	{
		conf_des->input_fd = open(conf_des->input_file, O_RDONLY);
		if (conf_des->input_fd == -1)
			return (perror(conf_des->input_file), des_free_exec(&exec_des), 1);
		if (dup2(conf_des->input_fd, STDIN_FILENO) == -1)
			return (close(conf_des->input_fd), des_free_exec(&exec_des), perror_int());
		if (close(conf_des->input_fd))
			return (des_free_exec(&exec_des), perror_int());
	}
	{
		char	read_buffer[DES_BUFFER_SIZE];

		while ((bytes_read = read(STDIN_FILENO, read_buffer, DES_BUFFER_SIZE)) > 0)
		{
			if (add_to_input_buffer(read_buffer, bytes_read, &exec_des.input_buffer_size, &exec_des.input_buffer))
				return (des_free_exec(&exec_des), perror_int());
		}
		if (bytes_read == -1)
			return (des_free_exec(&exec_des), perror_int());
	}
	if (conf_des->flags & FLAG_DES_OUTPUT_FILE)
	{
		conf_des->output_fd = open(conf_des->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (conf_des->output_fd == -1)
			return (perror(conf_des->output_file), des_free_exec(&exec_des), 1); 
		if (dup2(conf_des->output_fd, STDOUT_FILENO) == -1)
			return (close(conf_des->output_fd), des_free_exec(&exec_des), perror_int());
		if (close(conf_des->output_fd))
			return (des_free_exec(&exec_des), perror_int());
	}
	if (conf_des->flags & FLAG_DES_BASE64 && conf_des->flags & FLAG_DES_DECRYPT)
	{
		if (base64_check_encoded(exec_des.input_buffer, exec_des.input_buffer_size))
			return (fprintf(stderr, "%s: Error: invalid base64 input\n", FT_SSL_NAME), des_free_exec(&exec_des), 1);
		temp_buffer = base64_decode(exec_des.input_buffer, exec_des.input_buffer_size, &exec_des.input_buffer_size);
		free(exec_des.input_buffer);
		exec_des.input_buffer = NULL;
		if (!temp_buffer)
			return (des_free_exec(&exec_des), perror_int());
		exec_des.input_buffer = temp_buffer;
	}
	if ((conf_des->flags & FLAG_DES_DECRYPT) ? des_execute_decipher(&exec_des) : des_execute_cipher(&exec_des))
		return (des_free_exec(&exec_des), 1);
	if (conf_des->flags & FLAG_DES_BASE64 && !(conf_des->flags & FLAG_DES_DECRYPT))
	{
		temp_buffer = base64_encode(exec_des.output_buffer, exec_des.output_buffer_size, &exec_des.output_buffer_size);
		free(exec_des.output_buffer);
		exec_des.output_buffer = NULL;
		if (!temp_buffer)
			return (des_free_exec(&exec_des), perror_int());
		exec_des.output_buffer = temp_buffer;
	}
	if (conf_des->flags & FLAG_DES_BASE64 && !(conf_des->flags & FLAG_DES_DECRYPT))
	{
		for (size_t i = 0; i < exec_des.output_buffer_size; i += 64)
		{
			if (exec_des.output_buffer_size - i >= 64)
			{
				if (write(STDOUT_FILENO, exec_des.output_buffer + i, 64) == -1)
					return (des_free_exec(&exec_des), perror_int());
			}
			else
			{
				if (write(STDOUT_FILENO, exec_des.output_buffer + i, exec_des.output_buffer_size - i) == -1)
					return (des_free_exec(&exec_des), perror_int());
			}
			if (write(STDOUT_FILENO, "\n", 1) == -1)
					return (des_free_exec(&exec_des), perror_int());
		}
	}
	else if (write(STDOUT_FILENO, exec_des.output_buffer, exec_des.output_buffer_size) == -1)
		return (des_free_exec(&exec_des), perror_int());
	des_free_exec(&exec_des);
	return (0);
}