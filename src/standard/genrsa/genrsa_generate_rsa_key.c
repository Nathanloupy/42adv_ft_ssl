#include "commons.h"

#define MAX_32BIT_PRIME 0xFFFFFFFF
#define MIN_PRIME_VALUE 1000000

static u_int64_t	choose_prime()
{
	int			fd;
	u_int64_t	num;

	fd = open("/dev/urandom", O_RDONLY);
	if (fd == -1)
		return (0);
	
	while (1)
	{
		if (read(fd, &num, sizeof(num)) != sizeof(num))
		{
			close(fd);
			return (0);
		}
		num = (num % (MAX_32BIT_PRIME - MIN_PRIME_VALUE)) + MIN_PRIME_VALUE;
		
		if (num % 2 == 0)
			num++;
		if (check_prime(num, GENRSA_PRIME_PROBABILITY))
		{
			close(fd);
			return (num);
		}
	}
}

static int mul_overflow_protected(u_int64_t a, u_int64_t b, u_int64_t *result)
{
	if (a == 0 || b == 0)
		return (0);
	*result = a * b;
	if (a != *result / b)
		return (1);
	return (0);
}

int	generate_rsa_key(t_rsa_key *rsa_key)
{
	u_int64_t	n_result;
	int			flag_perror = 0;

	for (int i = 0; i < GENRSA_MAX_PRIME_GENERATION_ATTEMPTS; i++)
	{
		rsa_key->p = choose_prime();
		rsa_key->q = choose_prime();
		if (rsa_key->p == 0 || rsa_key->q == 0)
		{
			flag_perror = 1;
			continue;
		}
		if (rsa_key->p == rsa_key->q || mul_overflow_protected(rsa_key->p, rsa_key->q, &n_result))
		{
			flag_perror = 0;
			continue;
		}
		rsa_key->n = n_result;
		rsa_key->e = 65537;
		rsa_key->d = mod_inv(rsa_key->e, lcm(rsa_key->p - 1, rsa_key->q - 1));
		return (0);
	}
	if (flag_perror)
		return (perror_int());
	return (fprintf(stderr, "%s: Error: Failed to generate RSA key after %d attempts\n", FT_SSL_NAME, GENRSA_MAX_PRIME_GENERATION_ATTEMPTS), 1);
}