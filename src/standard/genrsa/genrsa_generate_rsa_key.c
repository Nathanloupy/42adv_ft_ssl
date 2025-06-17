#include "commons.h"

static u_int64_t	choose_prime(int urandom_fd)
{
	u_int64_t	num;

	for (int i = 0; i < GENRSA_MAX_PRIME_CHOICE_ATTEMPTS; i++)
	{
		if (read(urandom_fd, &num, sizeof(num)) != sizeof(num))
			return (0);
		num = (num % (GENRSA_MAX_PRIME_VALUE - GENRSA_MIN_PRIME_VALUE)) + GENRSA_MIN_PRIME_VALUE;
		if (num % 2 == 0)
			num++;
		if (check_prime(num, GENRSA_PRIME_PROBABILITY, urandom_fd))
			return (num);
	}
	return (0);
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
	u_int64_t	phi_n_result;
	int			flag_perror = 0;
	int			urandom_fd;

	urandom_fd = open("/dev/urandom", O_RDONLY);
	if (urandom_fd == -1)
		return (perror_int());

	for (int i = 0; i < GENRSA_MAX_KEY_GENERATION_ATTEMPTS; i++)
	{
		rsa_key->p = choose_prime(urandom_fd);
		rsa_key->q = choose_prime(urandom_fd);
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
		if (mul_overflow_protected(rsa_key->p - 1, rsa_key->q - 1, &phi_n_result))
		{
			flag_perror = 0;
			continue;
		}
		if (gcd(rsa_key->e, phi_n_result) != 1)
		{
			flag_perror = 0;
			continue;
		}
		rsa_key->d = mod_inv(rsa_key->e, phi_n_result);
		if (rsa_key->d == 0)
		{
			flag_perror = 0;
			continue;
		}
		return (close(urandom_fd), 0);
	}
	close(urandom_fd);
	if (flag_perror)
		return (perror_int());
	return (fprintf(stderr, "%s: Error: Failed to generate RSA key after %d attempts\n", FT_SSL_NAME, GENRSA_MAX_KEY_GENERATION_ATTEMPTS), 1);
}