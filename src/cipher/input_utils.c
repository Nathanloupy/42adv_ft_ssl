#include "commons.h"

int	add_to_input_buffer(char *buffer, size_t size, size_t *input_size, char **input)
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