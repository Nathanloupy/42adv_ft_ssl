#pragma once

#include "libs.h"
#include "md5.h"
#include "sha256.h"

#define HANDLERS {
	{
		"md5",
		digest_parser,
		NULL
	},
	{
		"sha256",
		digest_parser,
		NULL
	},
	{
		NULL,
		NULL,
		NULL
	}
}

typedef union u_conf {
	t_conf_digest digest;
} t_conf;
