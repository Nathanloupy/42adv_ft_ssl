#pragma once

#include "commons.h"

/* UTILS */
int			check_prime(u_int64_t n, float probability);
u_int64_t	mod_exp(u_int64_t base, u_int64_t exp, u_int64_t mod);
u_int64_t	mod_inv(u_int64_t a, u_int64_t m);

#include "genrsa/genrsa.h"