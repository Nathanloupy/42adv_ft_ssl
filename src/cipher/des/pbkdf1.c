#include "commons.h"

//reports error to stderr and returns NULL
char	*pbkdf1(char *password, char *salt, size_t iterations, size_t key_length)
{
	char	*key = NULL;
	char	*derived_key = NULL;
	char	*derived_key_tmp = NULL;
	t_md5	md5;
	size_t	password_len;
	size_t	salt_len;
	size_t	concat_len;

	if (!password || !salt || !iterations || !key_length || key_length > 16)
		return(fprintf(stderr, "Error: Invalid parameters for PBKDF1\n"), NULL);

	password_len = strlen(password);
	salt_len = strlen(salt);
	concat_len = password_len + salt_len;

	derived_key = calloc(concat_len + 1, sizeof(char));
	if (!derived_key)
		return (perror_ptr());
	memcpy(derived_key, password, password_len);
	memcpy(derived_key + password_len, salt, salt_len);

	md5_init(&md5);
	derived_key_tmp = md5_process(&md5, (unsigned char *)derived_key, concat_len, 1);
	if (!derived_key_tmp)
		return (free(derived_key), perror_ptr());
	free(derived_key);
	derived_key = derived_key_tmp;

	for (size_t i = 1; i < iterations; i++)
	{
		md5_init(&md5);
		derived_key_tmp = md5_process(&md5, (unsigned char *)derived_key, 32, 1);
		if (!derived_key_tmp)
			return (free(derived_key), perror_ptr());
		free(derived_key);
		derived_key = derived_key_tmp;
	}

	key = calloc(key_length + 1, sizeof(char));
	if (!key)
		return (free(derived_key), perror_ptr());

	char hex_byte[3];
	for (size_t i = 0; i < key_length; i++)
	{
		hex_byte[0] = derived_key[i * 2];
		hex_byte[1] = derived_key[i * 2 + 1];
		hex_byte[2] = '\0';
		key[i] = (char)strtol(hex_byte, NULL, 16);
	}
	free(derived_key);
	return (key);
}