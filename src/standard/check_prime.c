#include "commons.h"

static int miller_rabin_witness(u_int64_t n, u_int64_t a, u_int64_t d, int r)
{
	u_int64_t x = mod_exp(a, d, n);
	
	if (x == 1 || x == n - 1)
		return (1);
	
	for (int i = 0; i < r - 1; i++)
	{
		x = mod_exp(x, 2, n);
		if (x == n - 1)
			return (1);
	}
	return (0);
}

static u_int64_t random_range(u_int64_t min, u_int64_t max, int urandom_fd)
{	
	u_int64_t	random_num;
	
	if (min >= max)
		return (min);
	
	if (read(urandom_fd, &random_num, sizeof(random_num)) != sizeof(random_num))
		return (perror_int(), 0);
	
	u_int64_t range = max - min;
	if (range == ULLONG_MAX)
		return min + random_num;
	return (min + random_num % (range + 1));
}

int check_prime(u_int64_t n, float probability, int urandom_fd)
{
	u_int64_t	d;
	u_int64_t	a;
	int			r;
	int			k;

	if (probability < 0.0 || probability > 1.0 || n <= 1 || (n % 2 == 0 && n != 2))
		return (0);

	d = n - 1;
	r = 0;
	while (d % 2 == 0)
	{
		d /= 2;
		r++;
	}

	k = ceil(log(1 - probability) / log(1.0 / 4.0));
	for (int i = 0; i < k; i++)
	{
		a = random_range(2, n - 2, urandom_fd);
		if (!a)
			return (0);
		if (!miller_rabin_witness(n, a, d, r))
			return (0);
	}
	return (1);
}