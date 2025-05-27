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

//TODO: check all returns for missing frees/closes/etc
int	des_executor(t_conf *conf)
{
	t_conf_des	*conf_des = (t_conf_des *)conf;
	t_exec_des	exec_des;
	char		temp_str[17];
	char		*temp_buffer;
	ssize_t		bytes_read;

	memset(&exec_des, 0, sizeof(t_exec_des));
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
	else
	{
		char	buffer[DES_BUFFER_SIZE];

		while ((bytes_read = read(STDIN_FILENO, buffer, DES_BUFFER_SIZE)) > 0)
		{
			if (add_to_input_buffer(buffer, bytes_read, &exec_des.input_buffer_size, &exec_des.input_buffer))
				return (des_free_exec(&exec_des), perror_int());
		}
		if (bytes_read == -1)
			return (perror_int());
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
	//!
	//TODO: execute the cipher/decipher depending on the mode
	//!
	if (conf_des->flags & FLAG_DES_BASE64 && conf_des->flags & FLAG_DES_ENCRYPT)
	{
		temp_buffer = base64_encode(exec_des.output_buffer, exec_des.output_buffer_size, &exec_des.output_buffer_size);
		free(exec_des.output_buffer);
		exec_des.output_buffer = NULL;
		if (!temp_buffer)
			return (des_free_exec(&exec_des), perror_int());
		exec_des.output_buffer = temp_buffer;
	}
	if (write(STDOUT_FILENO, exec_des.output_buffer, exec_des.output_buffer_size) == -1)
		return (des_free_exec(&exec_des), perror_int());
	des_free_exec(&exec_des);
	return (0);
}