#include "commons.h"

int main(int argc, char **argv)
{
	t_conf conf;

	if (argc >= 2)
	{
		for (int i = 0; HANDLERS[i].name != NULL; i++)
		{
			if (strcmp(argv[1], HANDLERS[i].name) != 0)
				continue;
			HANDLERS[i].parser(argc - 1, argv + 1, &conf);
			HANDLERS[i].executor(&conf);
			HANDLERS[i].cleaner(&conf);
			return (0);
		}
	}
	print_commands(argv[1]);
	return (1);
}