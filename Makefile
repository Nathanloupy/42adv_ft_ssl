# Makefile for ft_ssl

CC = cc
CCFLAGS = -Wall -Wextra -Werror -lreadline

NAME = ft_ssl
BUILD_DIR = bin
NAME_DIR = $(addprefix $(BUILD_DIR)/, $(NAME))

INCLUDES = -I./include/ -I./include/utils/ -I./include/digest/ -I./include/cipher/

FILES_MAIN = main.c

FILES_UTILS = print_commands.c \
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
				sha256/sha256_executor.c

SRCS_MAIN = $(addprefix src/, $(FILES_MAIN))

SRCS_UTILS = $(addprefix src/utils/, $(FILES_UTILS))

SRCS_DIGEST = $(addprefix src/digest/, $(FILES_DIGEST))

OBJS_MAIN = $(SRCS_MAIN:.c=.o)

OBJS_UTILS = $(SRCS_UTILS:.c=.o)

OBJS_DIGEST = $(SRCS_DIGEST:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_UTILS) $(OBJS_DIGEST)

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