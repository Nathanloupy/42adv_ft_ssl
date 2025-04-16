#include "commons.h"

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

static int 	write_encoded(char *output, size_t output_size)
{	
	for (size_t i = 0; i < output_size; i += BASE64_BLOCK_SIZE)
	{
		size_t	remaining = output_size - i;
		size_t	write_size = remaining < BASE64_BLOCK_SIZE ? remaining : BASE64_BLOCK_SIZE;

		if (write(STDOUT_FILENO, output + i, write_size) == -1)
			return (-1);
		if (write(STDOUT_FILENO, "\n", 1) == -1)
			return (-1);
	}
	return (0);
}

int base64_executor(t_conf *conf)
{
	t_conf_base64 *conf_base64 = (t_conf_base64 *)conf;

	if (conf_base64->flags & FLAG_BASE64_HELP || conf_base64->flags & FLAG_BASE64_USAGE)
		return (0);
	if (conf_base64->flags & FLAG_BASE64_DECODE && conf_base64->flags & FLAG_BASE64_ENCODE)
		return (fprintf(stderr, "%s: Error: cannot encode and decode at the same time\n", FT_SSL_NAME), 1);
	if (conf_base64->flags & FLAG_BASE64_INPUT_FILE)
	{
		conf_base64->input_fd = open(conf_base64->input_file, O_RDONLY);
		if (conf_base64->input_fd == -1)
			return (perror(conf_base64->input_file), 1);
		if (dup2(conf_base64->input_fd, STDIN_FILENO) == -1)
			return (close(conf_base64->input_fd), perror_int());
		if (close(conf_base64->input_fd))
			return (perror_int());
	}
	if (conf_base64->flags & FLAG_BASE64_OUTPUT_FILE)
	{
		conf_base64->output_fd = open(conf_base64->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0644);
		if (conf_base64->output_fd == -1)
			return (perror(conf_base64->output_file), 1);
		if (dup2(conf_base64->output_fd, STDOUT_FILENO) == -1)
			return (close(conf_base64->output_fd), perror_int());
		if (close(conf_base64->output_fd))
			return (perror_int());
	}
	if (conf_base64->flags & FLAG_BASE64_DECODE)
	{
		char	buffer[BASE64_BUFFER_SIZE];
		ssize_t	bytes_read;
		size_t	output_size;
		char	*output;

		while ((bytes_read = read(STDIN_FILENO, buffer, BASE64_BUFFER_SIZE)) > 0)
		{
			if (base64_check_encoded(buffer, bytes_read))
				return (fprintf(stderr, "%s: Error: invalid base64 input\n", FT_SSL_NAME), 1);
			output = base64_decode(buffer, bytes_read, &output_size);
			if (!output)
				return (perror_int());
			if (write(STDOUT_FILENO, output, output_size) == -1)
				return (free(output), perror_int());
			free(output);
		}
		if (bytes_read == -1)
			return (perror_int());
	}
	else
	{
		char	buffer[BASE64_BUFFER_SIZE];
		ssize_t	bytes_read;
		size_t	input_size = 0;
		char	*input = NULL;
		size_t	output_size = 0;
		char	*output = NULL;

		while ((bytes_read = read(STDIN_FILENO, buffer, BASE64_BUFFER_SIZE)) > 0)
		{
			if (add_to_input_buffer(buffer, bytes_read, &input_size, &input))
				return (free(input), perror_int());
		}
		if (bytes_read == -1)
			return (perror_int());
		output = base64_encode(input, input_size, &output_size);
		if (!output)
			return (free(input), perror_int());
		if (write_encoded(output, output_size) == -1)
			return (free(input), free(output), perror_int());
		free(input);
		free(output);
	}
	return (0);
}
