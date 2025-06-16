#include "commons.h"

u_int64_t	mod_exp(u_int64_t base, u_int64_t exp, u_int64_t mod)
{
	u_int64_t result = 1;
	
	if (mod <= 1)
		return (0);

	base = base % mod;
	while (exp > 0)
	{
		if (exp & 1)
			result = ((__uint128_t)result * base) % mod;
		exp = exp >> 1;
		base = ((__uint128_t)base * base) % mod;
	}
	return result;
}

u_int64_t	mod_inv(u_int64_t a, u_int64_t m)
{
	(void)a;
	(void)m;
	return (0);
}