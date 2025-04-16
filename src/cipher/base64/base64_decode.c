#include "commons.h"

static char	base64_get_index(char c)
{
	for (size_t i = 0; i < sizeof(BASE64_ALPHABET); i++)
	{
		if (c == BASE64_ALPHABET[i])
			return (i);
	}
	return (sizeof(BASE64_ALPHABET));
}

static void	base64_decode_nbytes(char (*decoded)[3], const char *buffer, size_t size)
{
	char	indexes[4];
	size_t	j = 0;

	for (size_t i = 0; i < 4; i++)
	{
		while (j < size && is_whitespace(buffer[j]))
			j++;
		if (j >= size)
			indexes[i] = 0;
		else if (buffer[j] == '=')
			indexes[i] = 0;
		else
			indexes[i] = base64_get_index(buffer[j]);
		j++;
	}
	(*decoded)[0] = ((indexes[0] & 0x3F) << 2) | ((indexes[1] & 0x30) >> 4);
	(*decoded)[1] = ((indexes[1] & 0x0F) << 4) | ((indexes[2] & 0x3C) >> 2);
	(*decoded)[2] = ((indexes[2] & 0x03) << 6) | (indexes[3] & 0x3F);
}

char	*base64_decode(const char *buffer, size_t size, size_t *output_size)
{	
	char	*decoded;
	char	temp[3];
	size_t	remaining;
	size_t	decoded_size;
	size_t	valid_chars = 0;

	for (size_t i = 0; i < size; i++)
	{
		if (!is_whitespace(buffer[i]))
			valid_chars++;
	}
	decoded_size = valid_chars * 3 / 4 + 1;
	decoded = calloc(decoded_size, sizeof(char));
	if (!decoded)
		return (NULL);
	decoded_size = 0;
	for (size_t i = 0; i < size; i += 4)
	{
		remaining = size - i;
		base64_decode_nbytes(&temp, buffer + i, remaining);
		memcpy(decoded + decoded_size, temp, 3);
		decoded_size += 3;
	}
	*output_size = decoded_size;
	return (decoded);
}

int	base64_check_encoded(const char *buffer, size_t size)
{
	int		has_padding = 0;
	size_t	valid_chars = 0;

	if (!buffer || size == 0)
		return (1);
	for (size_t i = 0; i < size; i++)
	{
		if (is_whitespace(buffer[i]))
			continue;
		valid_chars++;
		if (buffer[i] == '=')
		{
			has_padding = 1;
			if (i < size - 2)
				return (1);
		}
		else if (has_padding)
			return (1);
		else if (!(base64_get_index(buffer[i]) < sizeof(BASE64_ALPHABET)))
			return (1);
	}
	if (valid_chars % 4 != 0)
		return (1);
	return (0);
}