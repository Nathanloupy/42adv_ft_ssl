#include "commons.h"

int	md5_executor(t_conf *conf)
{
	t_conf_digest	*conf_digest = (t_conf_digest *)conf;
	t_md5			md5;
	char			*result = NULL;
	char			*input = NULL;
	unsigned char	buffer[MD5_BLOCK_SIZE];
	ssize_t			bytes_read;

	if (conf_digest->flags & FLAG_DIGEST_HELP || conf_digest->flags & FLAG_DIGEST_USAGE)
		return (0);
	if (conf_digest->flags & FLAG_DIGEST_ECHO || (!(conf_digest->flags & FLAG_DIGEST_STRING) && lstsize(conf_digest->files) == 0))
	{
		size_t			total_bytes = 0;

		md5_init(&md5);
		memset(buffer, 0, MD5_BLOCK_SIZE);
		while ((bytes_read = read(STDIN_FILENO, buffer, MD5_BLOCK_SIZE)) > 0)
		{
			if (digest_add_to_input(&input, buffer, bytes_read, total_bytes))
			{
				if (input)
					free(input);
				return (1);
			}
			total_bytes += bytes_read;
		}
		if (bytes_read == -1)
		{
			perror(FT_SSL_NAME);
			return (1);
		}
		result = md5_process(&md5, (unsigned char *)input, total_bytes, 1);
		conf_digest->stdin_size = total_bytes;
		if (!result)
		{
			if (input)
				free(input);
			return (1);
		}
		digest_print_result(result, conf_digest, DIGEST_NAME_MD5, input, INPUT_DIGEST_TYPE_STDIN);
		if (input)
			free(input);
		free(result);
		result = NULL;
	}
	if (conf_digest->flags & FLAG_DIGEST_STRING)
	{
		md5_init(&md5);
		memset(buffer, 0, MD5_BLOCK_SIZE);
		result = md5_process(&md5, (unsigned char *)conf_digest->string, strlen(conf_digest->string), 1);
		if (!result)
			return (1);
		digest_print_result(result, conf_digest, DIGEST_NAME_MD5, conf_digest->string, INPUT_DIGEST_TYPE_STRING);
		free(result);
		result = NULL;
	}
	if (lstsize(conf_digest->files) > 0)
	{
		t_list *current = conf_digest->files;
		while (current)
		{
			md5_init(&md5);
			memset(buffer, 0, MD5_BLOCK_SIZE);
			int fd = open(current->data, O_RDONLY);
			if (fd == -1)
			{
				perror(current->data);
				current = current->next;
				conf_digest->recoverable_error = 1;
				continue;
			}
			while ((bytes_read = read(fd, buffer, MD5_BLOCK_SIZE)) > 0)
			{
				if (bytes_read == MD5_BLOCK_SIZE)
					md5_process(&md5, buffer, bytes_read, 0);
				else
					result = md5_process(&md5, buffer, bytes_read, 1);
			}
			if (bytes_read == -1)
			{
				perror(current->data);
				current = current->next;
				conf_digest->recoverable_error = 1;
				continue;
			}
			if (bytes_read == 0 && !result)
			{
				result = md5_process(&md5, buffer, 0, 1);
			}
			close(fd);
			if (!result)
				return (1);
			digest_print_result(result, conf_digest, DIGEST_NAME_MD5, (char *)current->data, INPUT_DIGEST_TYPE_FILE);
			free(result);
			result = NULL;
			current = current->next;
		}
	}
	return (0);
}