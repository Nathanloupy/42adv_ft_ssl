#pragma once

#include "commons.h"

#define WHIRLPOOL_BLOCK_SIZE 64
#define WHIRLPOOL_ROUNDS 10

typedef struct s_whirlpool {
	u_int64_t	data_size;
	u_int8_t	hash_matrix[8][8];
}	t_whirlpool;

/* WHIRLPOOL - MAIN */
void	whirlpool_init(t_whirlpool *whirlpool);
char	*whirlpool_process(t_whirlpool *whirlpool, unsigned char *buffer, size_t buffer_size, int process_last_block);
int		whirlpool_executor(t_conf *conf);

/* WHIRLPOOL - UTILS */
