# Makefile for ft_ssl

CC = cc
CCFLAGS = -Wall -Wextra -Werror

NAME = ft_ssl
BUILD_DIR = bin
NAME_DIR = $(addprefix $(BUILD_DIR)/, $(NAME))

INCLUDES = -I./include/ -I./include/digest/

FILES_MAIN = main.c \
				print_commands.c

FILES_DIGEST = digest_parser.c

FILES_MD5 = md5.c

SRCS_MAIN = $(addprefix src/, $(FILES_MAIN))

SRCS_DIGEST = $(addprefix src/digest/, $(FILES_DIGEST))

SRCS_MD5 = $(addprefix src/digest/md5/, $(FILES_MD5))

OBJS_MAIN = $(SRCS_MAIN:.c=.o)

OBJS_DIGEST = $(SRCS_DIGEST:.c=.o)

OBJS_MD5 = $(SRCS_MD5:.c=.o)

OBJS = $(OBJS_MAIN) $(OBJS_DIGEST) $(OBJS_MD5)

%.o: %.c
	@$(CC) $(CCFLAGS) $(INCLUDES) -c $< -o $@

all: $(NAME)

$(NAME): $(OBJS)
	@mkdir -p $(BUILD_DIR)
	@$(CC) $(CCFLAGS) $(INCLUDES) $(OBJS) -o $(NAME_DIR) -lm

clean:
	@rm -f $(OBJS)

fclean: clean
	@rm -rf $(BUILD_DIR)

re: fclean all

run: all
	@./$(NAME_DIR)

.PHONY: all clean fclean re run