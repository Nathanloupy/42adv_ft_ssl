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

	for (size_t i = 0; i < 4; i++)
	{
		if (i >= size)
			indexes[i] = 0;
		else if (buffer[i] == '=')
			indexes[i] = 0;
		else
			indexes[i] = base64_get_index(buffer[i]);
	}
	(*decoded)[0] = ((indexes[0] & 0x3F) << 2) | ((indexes[1] & 0x30) >> 4);
	(*decoded)[1] = ((indexes[1] & 0x0F) << 4) | ((indexes[2] & 0x3C) >> 2);
	(*decoded)[2] = ((indexes[2] & 0x03) << 6) | (indexes[3] & 0x3F);
}

static char	*base64_decode_raw(const char *buffer, size_t size, size_t *output_size)
{	
	char	*decoded;
	char	temp[3];
	size_t	remaining;
	size_t	decoded_size;
	size_t	cpy_size;

	decoded_size = size * 3 / 4 + 3;
	decoded = calloc(decoded_size + 1, sizeof(char));
	if (!decoded)
		return (NULL);
	decoded_size = 0;
	for (size_t i = 0; i < size; i += 4)
	{
		remaining = size - i;
		base64_decode_nbytes(&temp, buffer + i, remaining);
		cpy_size = remaining >= 4 ? 3 : remaining * 3 / 4;
		memcpy(decoded + decoded_size, temp, cpy_size);
		decoded_size += cpy_size;
	}
	*output_size = decoded_size;
	return (decoded);
}

char	*base64_decode(const char *buffer, size_t size, size_t *output_size)
{	
	char	*decoded;
	char	*new_input;
	size_t	new_size = 0;

	for (size_t i = 0; i < size; i++)
	{
		if (!is_whitespace(buffer[i]) && buffer[i] != '=')
			new_size++;
	}
	new_input = calloc(new_size + 1, sizeof(char));
	if (!new_input)
		return (NULL);
	new_size = 0;
	for (size_t i = 0; i < size; i++)
	{
		if (!is_whitespace(buffer[i]) && buffer[i] != '=')
			new_input[new_size++] = buffer[i];
	}
	decoded = base64_decode_raw(new_input, new_size, output_size);
	free(new_input);
	return (decoded);
}

int	base64_check_encoded(const char *buffer, size_t size)
{
	int		padding = 0;
	size_t	valid_chars = 0;

	if (!buffer || size == 0)
		return (0);
	for (size_t i = 0; i < size; i++)
	{
		if (is_whitespace(buffer[i]))
			continue;
		valid_chars++;
		if (buffer[i] == '=')
			padding++;
		else if (padding)
			return (1);
		else if (!(base64_get_index(buffer[i]) < (char)sizeof(BASE64_ALPHABET)))
			return (1);
	}
	if (valid_chars % 4 != 0 || padding > 2)
		return (1);
	return (0);
}