#include "commons.h"

static int	print_rsa_private_key(t_rsa_key *rsa_key)
{
	char	*encoded_key;
	size_t	encoded_key_size;

	encoded_key = encode_rsa_private_key(rsa_key, &encoded_key_size);
	if (!encoded_key)
		return (perror_int());

	if (write(STDOUT_FILENO, "Generating RSA private key, 64 bit long modulus\n", 48) == -1)
		return (free(encoded_key), perror_int());
	if (write(STDOUT_FILENO, ".............................++++++++++++\n", 42) == -1)
		return (free(encoded_key), perror_int());
	if (write(STDOUT_FILENO, ".....................++++++++++++\n", 34) == -1)
		return (free(encoded_key), perror_int());
	if (write(STDOUT_FILENO, "e is 65537 (0x10001)\n", 21) == -1)
		return (free(encoded_key), perror_int());
	if (write(STDOUT_FILENO, "-----BEGIN RSA PRIVATE KEY-----\n", 32) == -1)
		return (free(encoded_key), perror_int());
	if (base64_write_encoded(encoded_key, encoded_key_size) == -1)
		return (free(encoded_key), perror_int());
	free(encoded_key);
	if (encoded_key_size % BASE64_BLOCK_SIZE == 0 && write(STDOUT_FILENO, "\n", 1) == -1)
		return (free(encoded_key), perror_int());
	if (write(STDOUT_FILENO, "-----END RSA PRIVATE KEY-----\n", 30) == -1)
		return (perror_int());
	return (0);
}

int genrsa_executor(t_conf *conf)
{
	t_conf_genrsa	*conf_genrsa = (t_conf_genrsa *)conf;
	t_rsa_key		rsa_key;

	if (conf_genrsa->flags & FLAG_GENRSA_HELP || conf_genrsa->flags & FLAG_GENRSA_USAGE)
		return (0);

	if (conf_genrsa->flags & FLAG_GENRSA_OUTPUT_FILE)
	{
		conf_genrsa->output_fd = open(conf_genrsa->output_file, O_WRONLY | O_CREAT | O_TRUNC, 0600);
		if (conf_genrsa->output_fd == -1)
			return (perror(conf_genrsa->output_file), 1);
		if (dup2(conf_genrsa->output_fd, STDOUT_FILENO) == -1)
			return (close(conf_genrsa->output_fd), perror_int());
		if (close(conf_genrsa->output_fd))
			return (perror_int());
	}

	if (generate_rsa_key(&rsa_key))
		return (1);
	return (print_rsa_private_key(&rsa_key));
}
