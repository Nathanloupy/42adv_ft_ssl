#include "commons.h"

static void	base64_encode_nbytes(char (*encoded)[4], const char *buffer, size_t size)
{
	unsigned char	entry[3];
	unsigned char	code[4];

	for (size_t i = 0; i < 3; i++)
	{
		if (i < size)
			entry[i] = buffer[i];
		else
			entry[i] = 0;
	}
	code[0] = (entry[0] & 0xFC) >> 2;
	code[1] = ((entry[0] & 0x03) << 4) | ((entry[1] & 0xF0) >> 4);
	code[2] = ((entry[1] & 0x0F) << 2) | ((entry[2] & 0xC0) >> 6);
	code[3] = (entry[2] & 0x3F);
	for (size_t i = 0; i < 4; i++)
		(*encoded)[i] = BASE64_ALPHABET[(unsigned int)code[i] & 0x3F];
}

static void	base64_encode_remaining(char (*encoded)[4], const char *buffer, size_t size)
{
	base64_encode_nbytes(encoded, buffer, size);

	for (size_t i = size + 1; i <= 3; i++)
		(*encoded)[i] = '=';
}

char	*base64_encode(const char *buffer, size_t size, size_t *output_size)
{
	size_t	remaining;
	char	*encoded;
	size_t	encoded_size;
	char	temp[4];

	encoded_size = size * 4 / 3 + 4;
	encoded = calloc(encoded_size, sizeof(char));
	if (!encoded)
		return (NULL);
	for (size_t i = 0; i < size; i += 3)
	{
		remaining = size - i;
		if (remaining < 3)
			base64_encode_remaining(&temp, buffer + i, remaining);
		else
			base64_encode_nbytes(&temp, buffer + i, 3);
		memcpy(encoded + i * 4 / 3, temp, 4);
	}
	*output_size = (size + 2) / 3 * 4;
	return (encoded);
}