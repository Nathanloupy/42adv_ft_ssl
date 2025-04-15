#include "commons.h"

unsigned char	base64_get_index(char c)
{
	for (size_t i = 0; i < BASE64_ALPHABET_SIZE; i++)
	{
		if (BASE64_ALPHABET[i] == c)
			return (i);
	}
	return (BASE64_ALPHABET_SIZE);
}

static void	base64_decode_nbytes(char (*decoded)[3], const char *buffer, size_t size)
{
	unsigned char	indexes[4];

	for (size_t i = 0; i < 4; i++)
	{
		if (i >= size)
			indexes[i] = 0;
		else if (buffer[i] == '=')
			indexes[i] = 0;
		else
			indexes[i] = base64_get_index(buffer[i]);
	}
	decoded[0] = ((indexes[0] & 0xFC) << 2) | ((indexes[1] & 0xC0) >> 6);
	decoded[1] = ((indexes[1] & 0xF0) << 4) | ((indexes[2] & 0x0F) >> 4);
	decoded[2] = ((indexes[2] & 0xC0) << 6) | (indexes[3] & 0x3F);
}

char	*base64_decode(const char *buffer, size_t size)
{
	char	*decoded;
	char	temp[4];
	int		remaining;

	decoded = calloc(size * 3 / 4 + 4, sizeof(char));
	if (!decoded)
		return (NULL);
	for (size_t i = 0; i < size; i += 4)
	{
		remaining = size - i;
		base64_decode_nbytes(&temp, buffer + i, 4);
		memcpy(decoded + i * 3 / 4, temp, 3);
	}
	return (decoded);
}