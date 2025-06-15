#include "commons.h"

int genrsa_executor(t_conf *conf)
{
	t_conf_genrsa *conf_genrsa = (t_conf_genrsa *)conf;

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

	//TODO: Generate the RSA key and write it to stdout
	return (0);
}
