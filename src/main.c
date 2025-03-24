#include "commons.h"

int main(int argc, char **argv)
{
	t_conf conf;

	if (argc < 2)
	{
		//TODO: print global usage
		return (1);
	}
	for (int i = 0; HANDLERS[i][0] != NULL; i++)
	{
		if (strcmp(argv[1], HANDLERS[i][0]) != 0)
			continue;
		HANDLERS[i][1](argc - 1, argv + 1, &conf);
		return (0);
	}
	//TODO: print global usage
	return (1);
}