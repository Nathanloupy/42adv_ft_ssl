#include "commons.h"

void	digest_cleaner(t_conf *conf)
{
	t_conf_digest *conf_digest = (t_conf_digest *)conf;
	lstclear(&conf_digest->files);
}