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

static void	print_result(char *result, t_conf_digest *conf_digest, char *source_name, int input_type)
{
	switch (input_type)
	{
		case INPUT_DIGEST_TYPE_STDIN:
		{
			if (conf_digest->flags & FLAG_DIGEST_ECHO)
			{
				fflush(stdout);
				if (conf_digest->flags & FLAG_DIGEST_QUIET)
				{
					write(STDOUT_FILENO, source_name, conf_digest->stdin_size);
					write(STDOUT_FILENO, "\n", 1);
					write(STDOUT_FILENO, result, conf_digest->stdin_size);
					write(STDOUT_FILENO, "\n", 1);
				}
				else
				{
					write(STDOUT_FILENO, "(\"", 2);
					write(STDOUT_FILENO, source_name, conf_digest->stdin_size);
					write(STDOUT_FILENO, "\")= ", 4);
					write(STDOUT_FILENO, result, strlen(result));
					write(STDOUT_FILENO, "\n", 1);
				}
			}
			else if (conf_digest->flags & FLAG_DIGEST_QUIET)
				printf("%s\n", result);
			else
				printf("(stdin)= %s\n", result);
			break;
		}
		case INPUT_DIGEST_TYPE_STRING:
		{
			if (conf_digest->flags & FLAG_DIGEST_QUIET)
				printf("%s\n", result);
			else if (conf_digest->flags & FLAG_DIGEST_REVERSE)
				printf("%s \"%s\"\n", result, source_name);
			else
				printf("MD5 (\"%s\") = %s\n", source_name, result);
			break;
		}
		case INPUT_DIGEST_TYPE_FILE:
		{
			if (conf_digest->flags & FLAG_DIGEST_QUIET)
				printf("%s\n", result);
			else if (conf_digest->flags & FLAG_DIGEST_REVERSE)
				printf("%s %s\n", result, source_name);
			else
				printf("MD5 (%s) = %s\n", source_name, result);
			break;
		}
		default:
			return;
	}
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
		conf_digest->stdin_size = total_bytes;
		if (!result)
			return (1);
		print_result(result, conf_digest, input, INPUT_DIGEST_TYPE_STDIN);
		if (input)
			free(input);
		free(result);
		result = NULL;
	}
	if (conf_digest->flags & FLAG_DIGEST_STRING)
	{
		md5_init(&md5);
		result = md5_process(&md5, (unsigned char *)conf_digest->string, strlen(conf_digest->string), 1);
		if (!result)
			return (1);
		print_result(result, conf_digest, conf_digest->string, INPUT_DIGEST_TYPE_STRING);
		free(result);
		result = NULL;
	}
	if (lstsize(conf_digest->files) > 0)
	{
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
			print_result(result, conf_digest, (char *)current->data, INPUT_DIGEST_TYPE_FILE);
			free(result);
			result = NULL;
			current = current->next;
		}
	}
	return (0);
}