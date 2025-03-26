#include "commons.h"

void	*perror_ptr(void)
{
	perror(FT_SSL_NAME);
	return (NULL);
}

int	perror_int(void)
{
	perror(FT_SSL_NAME);
	return (1);
}