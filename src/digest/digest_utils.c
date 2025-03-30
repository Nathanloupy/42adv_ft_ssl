#include "commons.h"

int	digest_error(t_conf *conf, int recoverable)
{
	t_conf_digest	*digest = (t_conf_digest *)conf;

	if (recoverable)
		return (digest->recoverable_error);
	else
		return (digest->unrecoverable_error);
}

int	digest_add_to_input(char **input, unsigned char *buffer, size_t bytes_read, size_t total_bytes)
{
	char	*new_input;

	if (!*input)
	{
		new_input = calloc(bytes_read + 1, sizeof(char));
		if (!new_input)
			return (perror_int());
		memcpy(new_input, buffer, bytes_read);
		new_input[bytes_read] = '\0';
		*input = new_input;
	}
	else
	{	
		new_input = calloc(total_bytes + bytes_read + 1, sizeof(char));
		if (!new_input)
			return (perror_int());
		memcpy(new_input, *input, total_bytes);
		memcpy(new_input + total_bytes, buffer, bytes_read);
		new_input[total_bytes + bytes_read] = '\0';
		free(*input);
		*input = new_input;
	}
	return (0);
}