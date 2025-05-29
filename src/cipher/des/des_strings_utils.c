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
	//TODO: verify password entry
	len = strlen(input);
	passphrase = calloc(len + 1, sizeof(char));
	if (!passphrase)
		return (free(input), NULL);
	strncpy(passphrase, input, len);
	free(input);
	return (passphrase);
}

char	*des_generate_random_salt(void)
{
	char	*salt;
	int		fd;

	salt = calloc(9, sizeof(char));
	if (!salt)
		return (NULL);
	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (free(salt), NULL);
	if (read(fd, salt, 8) != 8)
		return (close(fd), free(salt), NULL);
	close(fd);
	return (salt);
}