/*
Extracted from :
RFC 2898              Password-Based Cryptography         September 2000
5.2 PBKDF2

   PBKDF2 applies a pseudorandom function (see Appendix B.1 for an
   example) to derive keys. The length of the derived key is essentially
   unbounded. (However, the maximum effective search space for the
   derived key may be limited by the structure of the underlying
   pseudorandom function. See Appendix B.1 for further discussion.)
   PBKDF2 is recommended for new applications.

   PBKDF2 (P, S, c, dkLen)

   Options:        PRF        underlying pseudorandom function (hLen
                              denotes the length in octets of the
                              pseudorandom function output)

   Input:          P          password, an octet string
                   S          salt, an octet string
                   c          iteration count, a positive integer
                   dkLen      intended length in octets of the derived
                              key, a positive integer, at most
                              (2^32 - 1) * hLen

   Output:         DK         derived key, a dkLen-octet string

   Steps:

      1. If dkLen > (2^32 - 1) * hLen, output "derived key too long" and
         stop.

      2. Let l be the number of hLen-octet blocks in the derived key,
         rounding up, and let r be the number of octets in the last
         block:

                   l = CEIL (dkLen / hLen) ,
                   r = dkLen - (l - 1) * hLen .

         Here, CEIL (x) is the "ceiling" function, i.e. the smallest
         integer greater than, or equal to, x.

      3. For each block of the derived key apply the function F defined
         below to the password P, the salt S, the iteration count c, and
         the block index to compute the block:

                   T_1 = F (P, S, c, 1) ,
                   T_2 = F (P, S, c, 2) ,
                   ...
                   T_l = F (P, S, c, l) ,

         where the function F is defined as the exclusive-or sum of the
         first c iterates of the underlying pseudorandom function PRF
         applied to the password P and the concatenation of the salt S
         and the block index i:

                   F (P, S, c, i) = U_1 \xor U_2 \xor ... \xor U_c

         where

                   U_1 = PRF (P, S || INT (i)) ,
                   U_2 = PRF (P, U_1) ,
                   ...
                   U_c = PRF (P, U_{c-1}) .

         Here, INT (i) is a four-octet encoding of the integer i, most
         significant octet first.

      4. Concatenate the blocks and extract the first dkLen octets to
         produce a derived key DK:

                   DK = T_1 || T_2 ||  ...  || T_l<0..r-1>

      5. Output the derived key DK.

   Note. The construction of the function F follows a "belt-and-
   suspenders" approach. The iterates U_i are computed recursively to
   remove a degree of parallelism from an opponent; they are exclusive-
   ored together to reduce concerns about the recursion degenerating
   into a small set of values.
*/

#include "commons.h"

/*
P is password, S is salt, c is iteration count, dkLen is intended length in octets of the derived key
Here we use WHIRLPOOL as the PRF
*/

static void int_to_4_bytes(unsigned char *out, size_t i)
{
	out[0] = (i >> 24) & 0xFF;
	out[1] = (i >> 16) & 0xFF;
	out[2] = (i >> 8) & 0xFF;
	out[3] = i & 0xFF;
}

static char *F(char *P, char *S, size_t c, size_t i, size_t hLen)
{
	t_whirlpool whirlpool;
	char *U_prev = NULL;
	char *U_curr = NULL;
	char *result = calloc(hLen, sizeof(char));
	
	if (!result)
		return (perror_ptr());
		
	// Prepare S || INT(i)
	size_t salt_len = strlen(S);
	size_t password_len = strlen(P);
	unsigned char *combined_buffer = calloc(password_len + salt_len + 4, sizeof(unsigned char));
	if (!combined_buffer)
		return (free(result), perror_ptr());
	
	// Create P || S || INT(i)
	memcpy(combined_buffer, P, password_len);
	memcpy(combined_buffer + password_len, S, salt_len);
	int_to_4_bytes(combined_buffer + password_len + salt_len, i);
	
	// Calculate U_1 = PRF(P, S || INT(i))
	whirlpool_init(&whirlpool);
	U_curr = whirlpool_process(&whirlpool, combined_buffer, password_len + salt_len + 4, 1);
	free(combined_buffer);
	
	// Copy U_1 to result
	memcpy(result, U_curr, hLen);
	
	// Calculate U_2 through U_c and XOR with result
	for (size_t j = 2; j <= c; j++)
	{
		free(U_prev);
		U_prev = U_curr;
		
		// Create P || U_{j-1}
		combined_buffer = calloc(password_len + hLen, sizeof(unsigned char));
		if (!combined_buffer)
		{
			free(U_prev);
			free(result);
			return (perror_ptr());
		}
		
		memcpy(combined_buffer, P, password_len);
		memcpy(combined_buffer + password_len, U_prev, hLen);
		
		whirlpool_init(&whirlpool);
		U_curr = whirlpool_process(&whirlpool, combined_buffer, password_len + hLen, 1);
		free(combined_buffer);
		
		// XOR U_j with result
		for (size_t k = 0; k < hLen; k++)
			result[k] ^= U_curr[k];
	}
	
	free(U_prev);
	free(U_curr);
	return (result);
}

char	*pbkdf2(char *P, char *S, size_t c, size_t dkLen)
{
	size_t hLen = 64;
	size_t l = (dkLen + hLen - 1) / hLen;
	size_t r = dkLen - (l - 1) * hLen;

	if (dkLen > ((1ULL << 32) - 1) * hLen)
		return (fprintf(stderr, "Error: pbkdf2: derived key too long\n"), NULL);
	char *derived_key = calloc(dkLen + 1, sizeof(char));
	if (!derived_key)
		return (perror_ptr());
		
	// Calculate T_i = F(P, S, c, i) for each block
	for (size_t i = 1; i <= l; i++)
	{
		char *T_i = F(P, S, c, i, hLen);
		if (!T_i)
			return (free(derived_key), perror_ptr());
		// Concatenate T_i to the derived key
		size_t block_size = (i == l) ? r : hLen;
		memcpy(derived_key + (i - 1) * hLen, T_i, block_size);
		free(T_i);
	}
	return derived_key;
}