#include "commons.h"

/*
P
RSAPrivateKey ::= SEQUENCE {
  version   Version,
  modulus   INTEGER,  -- n
  publicExponent    INTEGER,  -- e
  privateExponent   INTEGER,  -- d
  prime1    INTEGER,  -- p
  prime2    INTEGER,  -- q
  exponent1 INTEGER,  -- d mod (p-1)
  exponent2 INTEGER,  -- d mod (q-1)
  coefficient   INTEGER,  -- (inverse of q) mod p
}

Check with : openssl rsa -in key.pem -check -noout
*/

static size_t encode_asn1_length(unsigned char *output, size_t length)
{
	if (length < 0x80)
	{
		if (output)
			output[0] = (unsigned char)length;
		return (1);
	}
	else
	{
		size_t byte_count = 0;
		size_t temp = length;
		
		while (temp > 0)
		{
			byte_count++;
			temp >>= 8;
		}
		
		if (output)
		{
			output[0] = 0x80 | (unsigned char)byte_count;
			for (size_t i = 0; i < byte_count; i++)
				output[1 + i] = (unsigned char)(length >> (8 * (byte_count - 1 - i)));
		}
		return (1 + byte_count);
	}
}

static size_t encode_asn1_integer(unsigned char *output, u_int64_t value)
{
	unsigned char bytes[9];
	size_t byte_count = 0;
	
	if (value == 0)
	{
		if (output)
		{
			output[0] = 0x02;
			output[1] = 0x01;
			output[2] = 0x00;
		}
		return (3);
	}
	
	u_int64_t temp = value;
	while (temp > 0)
	{
		bytes[byte_count] = (unsigned char)(temp & 0xFF);
		temp >>= 8;
		byte_count++;
	}
	
	for (size_t i = 0; i < byte_count / 2; i++)
	{
		unsigned char swap = bytes[i];
		bytes[i] = bytes[byte_count - 1 - i];
		bytes[byte_count - 1 - i] = swap;
	}

	size_t final_byte_count = byte_count;
	if (bytes[0] & 0x80)
	{
		final_byte_count++;
		if (output)
		{
			output[0] = 0x02;
			size_t length_size = encode_asn1_length(output + 1, final_byte_count);
			output[1 + length_size] = 0x00;
			memcpy(output + 1 + length_size + 1, bytes, byte_count);
		}
	}
	else
	{
		if (output)
		{
			output[0] = 0x02;
			size_t length_size = encode_asn1_length(output + 1, final_byte_count);
			memcpy(output + 1 + length_size, bytes, byte_count);
		}
	}
	
	return (1 + encode_asn1_length(NULL, final_byte_count) + final_byte_count);
}

static size_t calculate_content_size(t_rsa_key *rsa_key)
{
	size_t content_size = 0;
	
	content_size += encode_asn1_integer(NULL, rsa_key->version);
	content_size += encode_asn1_integer(NULL, rsa_key->n);
	content_size += encode_asn1_integer(NULL, rsa_key->e);
	content_size += encode_asn1_integer(NULL, rsa_key->d);
	content_size += encode_asn1_integer(NULL, rsa_key->p);
	content_size += encode_asn1_integer(NULL, rsa_key->q);
	content_size += encode_asn1_integer(NULL, rsa_key->d_p);
	content_size += encode_asn1_integer(NULL, rsa_key->d_q);
	content_size += encode_asn1_integer(NULL, rsa_key->q_inv);
	
	return (content_size);
}

char	*encode_rsa_private_key(t_rsa_key *rsa_key, size_t *encoded_key_size)
{
	size_t			content_size;
	size_t			length_size;
	size_t			der_size;
	unsigned char	*der_buffer;
	unsigned char	*current_pos;
	char			*base64_encoded;
	
	if (!rsa_key || !encoded_key_size)
		return (NULL);

	content_size = calculate_content_size(rsa_key);
	length_size = encode_asn1_length(NULL, content_size);
	der_size = 1 + length_size + content_size;
	
	der_buffer = calloc(der_size, sizeof(unsigned char));
	if (!der_buffer)
		return (NULL);
	
	current_pos = der_buffer;
	*current_pos++ = 0x30;
	
	current_pos += encode_asn1_length(current_pos, content_size);
	current_pos += encode_asn1_integer(current_pos, rsa_key->version);
	current_pos += encode_asn1_integer(current_pos, rsa_key->n);
	current_pos += encode_asn1_integer(current_pos, rsa_key->e);
	current_pos += encode_asn1_integer(current_pos, rsa_key->d);
	current_pos += encode_asn1_integer(current_pos, rsa_key->p);
	current_pos += encode_asn1_integer(current_pos, rsa_key->q);
	current_pos += encode_asn1_integer(current_pos, rsa_key->d_p);
	current_pos += encode_asn1_integer(current_pos, rsa_key->d_q);
	current_pos += encode_asn1_integer(current_pos, rsa_key->q_inv);
	
	base64_encoded = base64_encode((char *)der_buffer, der_size, encoded_key_size);
	free(der_buffer);
	return (base64_encoded);
}