#include "commons.h"

char	*base64_encode(const char *buffer, size_t size)
{
	int		remaining;
	char	*encoded;
	char	temp[4];
	
	encoded = calloc(size * 4 / 3 + 4, sizeof(char));
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
	return (encoded);
}