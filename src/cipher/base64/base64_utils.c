#include "commons.h"

void	base64_init(t_conf_base64 *conf_base64)
{
	conf_base64->input_fd = -1;
	conf_base64->output_fd = -1;
}

int	base64_recoverable_error(t_conf *conf)
{
	t_conf_base64	*base64 = (t_conf_base64 *)conf;

	(void)base64;
	return (0);
}

void	print_bits(void *buffer, size_t size)
{
	const char		*COLORS[] = {"\033[0;31m", "\033[0;32m", "\033[0;33m", "\033[0;34m", "\033[0;35m", "\033[0;36m", "\033[0;37m"};
	const char		*RESET = "\033[0m";
	unsigned char	*byte_buffer = (unsigned char *)buffer;
	int				limit = size == 4 ? 5 : 7;
	static int		char_count = 0;
	static void		*last_buffer = NULL;
	static int		color_index = 0;

	if (last_buffer != byte_buffer)
	{
		last_buffer = byte_buffer;
		char_count = 0;
	}
	for (size_t i = 0; i < size; i++)
	{
		printf("%s", COLORS[color_index++ % 7]);
		for (int j = limit; j >= 0; j--)
		{
			printf("%d", (byte_buffer[i] >> j) & 1);
			char_count++;
			if (char_count == 6)
			{
				char_count = 0;
				printf(" ");
			}
		}
		printf(RESET);
	}
	printf("\n");
}