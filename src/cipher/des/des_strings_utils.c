#include "commons.h"

void	des_string_length_error(size_t size)
{
	if (size < 16)
		fprintf(stderr, "%s: hex string is too short, padding with zero bytes to length 16\n", FT_SSL_NAME);
	else if (size > 16)
		fprintf(stderr, "%s: hex string is too long, ignoring excess\n", FT_SSL_NAME);
}

int	des_check_hex(const char *str)
{
	if (!str)
		return (1);
	for (size_t i = 0; i < strlen(str); i++)
	{
		if (!((str[i] >= '0' && str[i] <= '9') ||
			  (str[i] >= 'a' && str[i] <= 'f') ||
			  (str[i] >= 'A' && str[i] <= 'F')))
			return (1);
	}
	return (0);
}

char	*des_read_passphrase_from_stdin(void)
{
	char	*passphrase;
	char	*input;
	size_t	len;

	input = getpass("Enter DES passphrase: ");
	if (!input)
		return (NULL); //TODO: check for all possible errors
	
	len = strlen(input);
	passphrase = calloc(len + 1, sizeof(char));
	if (!passphrase)
		return (free(input), NULL);
	strncpy(passphrase, input, len);
	free(input);
	return (passphrase);
}