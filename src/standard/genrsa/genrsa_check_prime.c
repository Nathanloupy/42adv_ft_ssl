#include "commons.h"

/*
	Check if a number is prime using the Miller-Rabin algorithm
	
	Parameters:
		n: The number to check
		probability: The probability of the number being prime between 0.0 and 1.0

	Returns:
		1 if the number is prime, 0 otherwise

	Error probability is at most (1/4)^k, so k = log(1-p) / log(1/4)
*/

static u_int64_t genrsa_mod_exp(u_int64_t base, u_int64_t exp, u_int64_t mod)
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

static int genrsa_miller_rabin_witness(u_int64_t n, u_int64_t a, u_int64_t d, int r)
{
	u_int64_t x = genrsa_mod_exp(a, d, n);
	
	if (x == 1 || x == n - 1)
		return (1);
	
	for (int i = 0; i < r - 1; i++)
	{
		x = genrsa_mod_exp(x, 2, n);
		if (x == n - 1)
			return (1);
	}
	return (0);
}

static u_int64_t genrsa_random_range(u_int64_t min, u_int64_t max)
{	
	if (min >= max)
		return (min);
	
	// Prevent overflow in range calculation
	u_int64_t range = max - min;
	if (range == ULLONG_MAX)  // Handle maximum range
		return min + (((u_int64_t)rand() << 32) | rand());
	
	return (min + rand() % (range + 1));
}

int genrsa_check_prime(u_int64_t n, float probability)
{
	u_int64_t	d;
	u_int64_t	a;
	int			r;
	int			k;
	static int	seed_initialized = 0;

	if (n <= 3)
		return (1);
	else if (probability < 0.0 || probability > 1.0 || n % 2 == 0)
		return (0);

	d = n - 1;
	r = 0;
	while (d % 2 == 0)
	{
		d /= 2;
		r++;
	}

	if (!seed_initialized)
	{
		srand(time(NULL));
		seed_initialized = 1;
	}

	k = ceil(log(1 - probability) / log(1.0 / 4.0));
	for (int i = 0; i < k; i++)
	{
		a = genrsa_random_range(2, n - 2);
		if (!genrsa_miller_rabin_witness(n, a, d, r))
			return (0);
	}
	return (1);
}