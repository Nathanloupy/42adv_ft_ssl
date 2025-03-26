#include "commons.h"

void print_complete_block(const unsigned char *block)
{
	printf("Binary representation of padded data:\n");
	for (size_t i = 0; i < MD5_BLOCK_SIZE; i++)
	{
		printf("%02x ", block[i]);
		if ((i + 1) % 16 == 0)
			printf("\n");
		else
			printf(" ");
	}
	printf("\n");
}