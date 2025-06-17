#include "commons.h"

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

	printf("Generating RSA private key, %d bit long modulus\n", GENRSA_BIT_LENGTH);
	printf(".............................++++++++++++\n");
	printf(".....................++++++++++++\n");
	printf("e is %ld (0x%lx)\n", rsa_key.e, rsa_key.e);
	printf("-----BEGIN RSA PRIVATE KEY-----\n");
	printf("MIIBOwIBAAJBAMLh8BxMEm/x+wDjpcMAeCANVFUfKdp9XR2H4VAnCK7b3x6SBD0v\n");
	printf("q/e5iyp+zPDMiG2A263x6eQCRbUOXMpU1txEWgCk4w==\n");
	printf("-----END RSA PRIVATE KEY-----\n");
	return (0);
}
