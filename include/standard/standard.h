#pragma once

#include "commons.h"

/* PRIME - UTILS */
u_int64_t	mod_exp(u_int64_t base, u_int64_t exp, u_int64_t mod);
int			check_prime(u_int64_t n, float probability);

#include "genrsa/genrsa.h"