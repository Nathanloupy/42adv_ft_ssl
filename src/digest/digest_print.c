#include "commons.h"

void	digest_print_result(char *result, t_conf_digest *conf_digest, const char *digester_name, char *source_name, int input_type)
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
					write(STDOUT_FILENO, result, strlen(result));
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
				printf("%s (\"%s\") = %s\n", digester_name, source_name, result);
			break;
		}
		case INPUT_DIGEST_TYPE_FILE:
		{
			if (conf_digest->flags & FLAG_DIGEST_QUIET)
				printf("%s\n", result);
			else if (conf_digest->flags & FLAG_DIGEST_REVERSE)
				printf("%s %s\n", result, source_name);
			else
				printf("%s (%s) = %s\n", digester_name, source_name, result);
			break;
		}
		default:
			return;
	}
}