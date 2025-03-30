#include "commons.h"

void	print_commands(char *command, int is_invalid)
{
	if (!command && is_invalid)
		fprintf(stderr, "ft_ssl: Invalid command ''.\n\n");
	else if (command && is_invalid)
		fprintf(stderr, "ft_ssl: Invalid command '%s'.\n\n", command);
	fprintf(stderr, "Message Digest commands\n");
	for (int i = 0; HANDLERS[i].name != NULL; i++)
	{
		if (HANDLERS[i].type == DIGEST)
			fprintf(stderr, "%s\n", HANDLERS[i].name);
	}
	fprintf(stderr, "\nCipher commands\n");
	for (int i = 0; HANDLERS[i].name != NULL; i++)
	{
		if (HANDLERS[i].type == CIPHER)
			fprintf(stderr, "%s\n", HANDLERS[i].name);
	}
	fprintf(stderr, "\n");
}