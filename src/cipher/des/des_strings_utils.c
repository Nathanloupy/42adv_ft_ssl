#include "commons.h"

void	des_string_length_error(size_t size)
{
	if (size < 16)
		fprintf(stderr, "%s: hex string is too short, padding with zero bytes\n", FT_SSL_NAME);
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
	char	*input;
	char	*allocated_input;
	char	*verify_input;

	input = getpass("Enter DES passphrase: ");
	if (!input)
		return (perror_ptr());
	allocated_input = strdup(input);
	if (!allocated_input)
		return (memset(input, 0, strlen(input)), perror_ptr());
	memset(input, 0, strlen(input));
	verify_input = getpass("Verify DES passphrase: ");
	if (!verify_input)
		return (free(allocated_input), perror_ptr());
	if (strcmp(allocated_input, verify_input) != 0)
		return (fprintf(stderr, "%s: verify failure\n", FT_SSL_NAME), free(allocated_input), free(verify_input), NULL);
	free(verify_input);
	return (allocated_input);
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

u_int64_t	des_hex_to_ull(const char *str)
{
	u_int64_t	ull;
	size_t		size;
	char		temp_fixed_str[17];

	size = strlen(str);
	if (size > 16)
		size = 16;
	memset(temp_fixed_str, '0', sizeof(temp_fixed_str));
	memcpy(temp_fixed_str, str, size);
	temp_fixed_str[size] = '\0';
	ull = strtoull(temp_fixed_str, NULL, 16);
	return (ull);
}