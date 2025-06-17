#pragma once

#include "commons.h"

#define CIPHER_NAME_BASE64 "BASE64"
#define CIPHER_NAME_DES "DES"

/* UTILS */
int	add_to_input_buffer(char *buffer, size_t size, size_t *input_size, char **input);

#include "base64/base64.h"
#include "des/des.h"
