#include "commons.h"

const t_handler HANDLERS[] = {
	{
		.name = "md5",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = md5_executor,
		.cleaner = digest_cleaner
	},
	{
		.name = "sha256",
		.type = DIGEST,
		.parser = digest_parser,
		.executor = sha256_executor,
		.cleaner = digest_cleaner
	},
	{
		.name = NULL,
		.type = VOID,
		.parser = NULL,
		.executor = NULL,
		.cleaner = NULL
	}
}; 