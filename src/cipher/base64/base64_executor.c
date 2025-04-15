#include "commons.h"

int base64_executor(t_conf *conf)
{
	if (conf->base64.flags & FLAG_BASE64_HELP || conf->base64.flags & FLAG_BASE64_USAGE)
	{
		return (0);
	}
	return (1);
}
