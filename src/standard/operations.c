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
	int64_t		old_r, r;
	int64_t		old_s, s;
	int64_t		old_t, t;
	int64_t		quotient, temp;

	if (m <= 1)
		return (0);

	old_r = a;
	r = m;
	old_s = 1;
	s = 0;
	old_t = 0;
	t = 1;

	while (r != 0)
	{
		quotient = old_r / r;
		
		temp = r;
		r = old_r - quotient * r;
		old_r = temp;
		
		temp = s;
		s = old_s - quotient * s;
		old_s = temp;
		
		temp = t;
		t = old_t - quotient * t;
		old_t = temp;
	}

	if (old_r != 1)
		return (0);

	if (old_s < 0)
		old_s += m;

	return ((u_int64_t)old_s);
}

u_int64_t	gcd(u_int64_t a, u_int64_t b)
{
	u_int64_t	temp;

	if (a == 0)
		return (b);
	if (b == 0)
		return (a);

	while (b != 0)
	{
		temp = b;
		b = a % b;
		a = temp;
	}
	return (a);
}