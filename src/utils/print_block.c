#include "commons.h"

void print_block(const unsigned char *block, size_t block_size, char format)
{
	for (size_t i = 0; i < block_size; i++)
	{
		if (format == 'h')
			printf("%02x ", block[i]);
		else
		{
			for (int j = 7; j >= 0; j--)
				printf("%d", (block[i] >> j) & 1);
		}
		if ((i + 1) % 16 == 0)
			printf("\n");
		else
			printf(" ");
	}
	printf("\n");
}