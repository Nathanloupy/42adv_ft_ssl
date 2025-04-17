#include "commons.h"

int	des_executor(t_conf *conf)
{
	t_conf_des	*des = (t_conf_des *)conf;

	(void)des;
	char *test = pbkdf2("0000000000000001", "0000000000000000", 1, 16);
	printf("%s\n", test);
	free(test);
	return (0);
}