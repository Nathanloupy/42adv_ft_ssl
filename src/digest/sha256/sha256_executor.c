#include "commons.h"

int	sha256_executor(t_conf *conf)
{
	t_conf_digest	*conf_digest = (t_conf_digest *)conf;
	t_sha256		sha256;
	char			*result = NULL;
	char			*input = NULL;
	unsigned char	buffer[SHA256_BLOCK_SIZE];
	size_t			bytes_read;
	
	if (conf_digest->flags & FLAG_DIGEST_ECHO || (!(conf_digest->flags & FLAG_DIGEST_STRING) && lstsize(conf_digest->files) == 0))
	{
		size_t			total_bytes = 0;

		sha256_init(&sha256);
		while ((bytes_read = read(STDIN_FILENO, buffer, SHA256_BLOCK_SIZE)) > 0)
		{
			digest_add_to_input(&input, buffer, bytes_read);
			total_bytes += bytes_read;
		}
		result = sha256_process(&sha256, buffer, total_bytes, 1);
		conf_digest->stdin_size = total_bytes;
		if (!result)
			return (1);
		digest_print_result(result, conf_digest, DIGEST_NAME_SHA256, input, INPUT_DIGEST_TYPE_STDIN);
		if (input)
			free(input);
		free(result);
		result = NULL;
	}
	if (conf_digest->flags & FLAG_DIGEST_STRING)
	{
		sha256_init(&sha256);
		result = sha256_process(&sha256, (unsigned char *)conf_digest->string, strlen(conf_digest->string), 1);
		if (!result)
			return (1);
		digest_print_result(result, conf_digest, DIGEST_NAME_SHA256, conf_digest->string, INPUT_DIGEST_TYPE_STRING);
		free(result);
		result = NULL;
	}
	if (lstsize(conf_digest->files) > 0)
	{
		t_list *current = conf_digest->files;
		while (current)
		{
			sha256_init(&sha256);
			int fd = open(current->data, O_RDONLY);
			if (fd == -1)
			{
				perror_int();
				current = current->next;
				continue;
			}
			while ((bytes_read = read(fd, buffer, SHA256_BLOCK_SIZE)) > 0)
			{
				if (bytes_read == SHA256_BLOCK_SIZE)
					sha256_process(&sha256, buffer, bytes_read, 0);
				else
					result = sha256_process(&sha256, buffer, bytes_read, 1);
			}
			if (bytes_read == 0 && !result)
			{
				result = sha256_process(&sha256, buffer, 0, 1);
			}
			close(fd);
			if (!result)
				return (1);
			digest_print_result(result, conf_digest, DIGEST_NAME_SHA256, (char *)current->data, INPUT_DIGEST_TYPE_FILE);
			free(result);
			result = NULL;
			current = current->next;
		}
	}
	return (0);
}