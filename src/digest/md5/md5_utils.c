#include "commons.h"

void print_complete_block(const unsigned char *block, char format)
{
	printf("Binary representation of padded data (little endian):\n");
	for (size_t i = 0; i < MD5_BLOCK_SIZE; i++)
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