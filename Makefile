# Makefile for ft_ssl

CC = cc
CCFLAGS = -Wall -Wextra -Werror -lm

NAME = ft_ssl
BUILD_DIR = bin
NAME_DIR = $(addprefix $(BUILD_DIR)/, $(NAME))

INCLUDES = -I./include/ -I./include/utils/ -I./include/digest/ -I./include/cipher/ -I./include/standard/

FILES_MAIN = main.c

FILES_UTILS = utils.c \
				print_commands.c \
				print_block.c \
				list.c \
				errors.c \
				get_next_line.c \
				get_next_line_utils.c

FILES_DIGEST = digest_parser.c \
				digest_cleaner.c \
				digest_print.c \
				digest_utils.c \
				md5/md5.c \
				md5/md5_executor.c \
				sha256/sha256.c \
				sha256/sha256_executor.c \
				whirlpool/whirlpool.c \
				whirlpool/whirlpool_executor.c

FILES_CIPHER = base64/base64_encode.c \
				base64/base64_decode.c \
				base64/base64_executor.c \
				base64/base64_parser.c \
				base64/base64_cleaner.c \
				base64/base64_utils.c \
				des/des_executor.c \
				des/des_execution.c \
				des/des_parser.c \
				des/des_cleaner.c \
				des/des_utils.c \
				des/des_strings_utils.c \
				des/des_derive_key.c \
				des/des_round_keys_generation.c \
				des/des_permutations.c \
				des/des_rounds.c \
				des/des_cipher_block.c \
				des/des_decipher_block.c

FILES_STANDARD = genrsa/genrsa_parser.c \
				genrsa/genrsa_executor.c \
				genrsa/genrsa_cleaner.c \
				genrsa/genrsa_utils.c \
				genrsa/genrsa_generate_rsa_key.c \
				check_prime.c \
				operations.c \

SRCS_MAIN = $(addprefix src/, $(FILES_MAIN))

SRCS_UTILS = $(addprefix src/utils/, $(FILES_UTILS))

SRCS_DIGEST = $(addprefix src/digest/, $(FILES_DIGEST))

SRCS_CIPHER = $(addprefix src/cipher/, $(FILES_CIPHER))

SRCS_STANDARD = $(addprefix src/standard/, $(FILES_STANDARD))

OBJS_MAIN = $(SRCS_MAIN:.c=.o)

OBJS_UTILS = $(SRCS_UTILS:.c=.o)

OBJS_DIGEST = $(SRCS_DIGEST:.c=.o)

OBJS_CIPHER = $(SRCS_CIPHER:.c=.o)

OBJS_STANDARD = $(SRCS_STANDARD:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_UTILS) $(OBJS_DIGEST) $(OBJS_CIPHER) $(OBJS_STANDARD)

%.o: %.c
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) -o $(NAME_DIR)

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all

run: all
	@./$(NAME_DIR)

.PHONY: all clean fclean re run