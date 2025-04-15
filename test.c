#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define BASE64_ALPHABET "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/"

unsigned char	base64_get_index(char c)
{
	for (size_t i = 0; i < sizeof(BASE64_ALPHABET); i++)
	{
		if (BASE64_ALPHABET[i] == c)
			return (i);
	}
	return (sizeof(BASE64_ALPHABET));
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
	(*decoded)[0] = ((indexes[0] & 0xFC) << 2) | ((indexes[1] & 0xC0) >> 6);
	(*decoded)[1] = ((indexes[1] & 0xF0) << 4) | ((indexes[2] & 0x0F) >> 4);
	(*decoded)[2] = ((indexes[2] & 0xC0) << 6) | (indexes[3] & 0x3F);
}

char	*base64_decode(const char *buffer, size_t size)
{
	
	char	*decoded;
	char	temp[3];
	int		remaining;

	decoded = calloc(size * 3 / 4 + 4, sizeof(char));
	if (!decoded)
		return (NULL);
	for (size_t i = 0; i < size; i += 4)
	{
		remaining = size - i;
		base64_decode_nbytes(&temp, buffer + i, remaining);
		memcpy(decoded + i * 3 / 4, temp, 3);
	}
	return (decoded);
}

int	main(void)
{
	char	*decoded;
	char	*test;

	test = "SGVsbG8gV29ybGQh";
	decoded = base64_decode(test, strlen(test));
	printf("%s\n", decoded);
	free(decoded);
	return (0);
}