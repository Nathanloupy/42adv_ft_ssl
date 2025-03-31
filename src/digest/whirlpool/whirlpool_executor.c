#include "commons.h"

int	whirlpool_executor(t_conf *conf)
{
	t_conf_digest	*conf_digest = (t_conf_digest *)conf;
	t_whirlpool		whirlpool;
	char			*result = NULL;
	char			*input = NULL;
	unsigned char	buffer[WHIRLPOOL_BLOCK_SIZE];
	size_t			bytes_read;
	
	if (conf_digest->flags & FLAG_DIGEST_ECHO || (!(conf_digest->flags & FLAG_DIGEST_STRING) && lstsize(conf_digest->files) == 0))
	{
		size_t			total_bytes = 0;

		whirlpool_init(&whirlpool);
		memset(buffer, 0, WHIRLPOOL_BLOCK_SIZE);
		while ((bytes_read = read(STDIN_FILENO, buffer, WHIRLPOOL_BLOCK_SIZE)) > 0)
		{
			if (digest_add_to_input(&input, buffer, bytes_read, total_bytes))
			{
				if (input)
					free(input);
				return (1);
			}
			total_bytes += bytes_read;
		}
		result = whirlpool_process(&whirlpool, buffer, total_bytes, 1);
		conf_digest->stdin_size = total_bytes;
		if (!result)
		{
			if (input)
				free(input);
			return (1);
		}
		digest_print_result(result, conf_digest, DIGEST_NAME_WHIRLPOOL, input, INPUT_DIGEST_TYPE_STDIN);
		if (input)
			free(input);
		free(result);
		result = NULL;
	}
	if (conf_digest->flags & FLAG_DIGEST_STRING)
	{
		whirlpool_init(&whirlpool);
		memset(buffer, 0, WHIRLPOOL_BLOCK_SIZE);
		result = whirlpool_process(&whirlpool, (unsigned char *)conf_digest->string, strlen(conf_digest->string), 1);
		if (!result)
			return (1);
		digest_print_result(result, conf_digest, DIGEST_NAME_WHIRLPOOL, conf_digest->string, INPUT_DIGEST_TYPE_STRING);
		free(result);
		result = NULL;
	}
	if (lstsize(conf_digest->files) > 0)
	{
		t_list *current = conf_digest->files;
		while (current)
		{
			whirlpool_init(&whirlpool);
			memset(buffer, 0, WHIRLPOOL_BLOCK_SIZE);
			int fd = open(current->data, O_RDONLY);
			if (fd == -1)
			{
				perror_int();
				current = current->next;
				conf_digest->recoverable_error = 1;
				continue;
			}
			while ((bytes_read = read(fd, buffer, WHIRLPOOL_BLOCK_SIZE)) > 0)
			{
				if (bytes_read == WHIRLPOOL_BLOCK_SIZE)
					whirlpool_process(&whirlpool, buffer, bytes_read, 0);
				else
					result = whirlpool_process(&whirlpool, buffer, bytes_read, 1);
			}
			if (bytes_read == 0 && !result)
			{
				result = whirlpool_process(&whirlpool, buffer, 0, 1);
			}
			close(fd);
			if (!result)
				return (1);
			digest_print_result(result, conf_digest, DIGEST_NAME_WHIRLPOOL, (char *)current->data, INPUT_DIGEST_TYPE_FILE);
			free(result);
			result = NULL;
			current = current->next;
		}
	}
	return (0);
}