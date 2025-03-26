#include "commons.h"

static int	add_to_input(char **input, unsigned char *buffer, size_t bytes_read)
{
	if (!*input)
	{
		*input = calloc(bytes_read + 1, sizeof(char));
		if (!*input)
			return (perror_int());
		memcpy(*input, buffer, bytes_read);
		(*input)[bytes_read] = '\0';
	}
	else
	{	
		*input = realloc(*input, strlen(*input) + bytes_read + 1);
		if (!*input)
			return (perror_int());
		memcpy(*input + strlen(*input), buffer, bytes_read);
		(*input)[strlen(*input) + bytes_read] = '\0';
	}
	return (0);
}

static void	print_result(char *result, t_conf_digest *conf_digest, int is_stdin, char *source_name)
{
	(void)conf_digest;
	(void)is_stdin;
	(void)result;
	printf("%s\n", source_name);
	printf("%s\n", result);
}

int	md5_executor(t_conf *conf)
{
	t_conf_digest	*conf_digest = (t_conf_digest *)conf;
	t_md5			md5;
	char			*result = NULL;
	char			*input = NULL;
	unsigned char	buffer[MD5_BLOCK_SIZE];
	size_t			bytes_read;
	
	if (conf_digest->flags & FLAG_DIGEST_ECHO || (!(conf_digest->flags & FLAG_DIGEST_STRING) && lstsize(conf_digest->files) == 0))
	{
		size_t			total_bytes = 0;

		md5_init(&md5);
		while ((bytes_read = read(STDIN_FILENO, buffer, MD5_BLOCK_SIZE)) > 0)
		{
			add_to_input(&input, buffer, bytes_read);
			total_bytes += bytes_read;
		}
		result = md5_process(&md5, buffer, total_bytes, 1);
		if (!result)
			return (1);
		print_result(result, conf_digest, 1, NULL);
		free(result);
		result = NULL;
	}
	if (conf_digest->flags & FLAG_DIGEST_STRING)
	{
		md5_init(&md5);
		result = md5_process(&md5, (unsigned char *)conf_digest->string, strlen(conf_digest->string), 1);
		if (!result)
			return (1);
		print_result(result, conf_digest, 0, conf_digest->string);
		free(result);
		result = NULL;
	}
	t_list *current = conf_digest->files;
	while (current)
	{
		md5_init(&md5);
		int fd = open(current->data, O_RDONLY);
		if (fd == -1)
		{
			perror_int();
			current = current->next;
			continue;
		}
		while ((bytes_read = read(fd, buffer, MD5_BLOCK_SIZE)) > 0)
		{
			if (bytes_read == MD5_BLOCK_SIZE)
				md5_process(&md5, buffer, bytes_read, 0);
			else
				result = md5_process(&md5, buffer, bytes_read, 1);
		}
		close(fd);
		if (!result)
			return (1);
		print_result(result, conf_digest, 0, (char *)current->data);
		free(result);
		result = NULL;
		current = current->next;
	}
	return (0);
}