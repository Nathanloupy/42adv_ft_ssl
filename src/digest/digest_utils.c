#include "commons.h"

int	digest_add_to_input(char **input, unsigned char *buffer, size_t bytes_read)
{
	if (!*input)
	{
		*input = calloc(bytes_read + 1, sizeof(char));
		if (!*input)
			return (perror_int());
		memcpy(*input, buffer, bytes_read);
		(*input)[bytes_read] = '\0';
	}
	else
	{	
		*input = realloc(*input, strlen(*input) + bytes_read + 1);
		if (!*input)
			return (perror_int());
		memcpy(*input + strlen(*input), buffer, bytes_read);
		(*input)[strlen(*input) + bytes_read] = '\0';
	}
	return (0);
}