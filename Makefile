NAME=libcursed.a
BUILD_DIR=./build
SRCS=curses_confirm.c curses_window.c curses_info.c
OBJS=$(SRCS:%.c=$(BUILD_DIR)/%.o)
CFLAGS=-Weverything -Werror -Wextra -Wall -Wpedantic -Ofast
LIBFT_DIR=../libft
LIBFT=$(LIBFT_DIR)/libft.a
LINKER=-lpthread -lft
INC=-I $(LIBFT_DIR)/include
CC=clang
RANLIB=ranlib

all: $(NAME)

$(NAME): $(BUILD_DIR) $(OBJS)
	$(AR) rc $(NAME) $(OBJS)
	$(RANLIB) $(NAME)

$(BUILD_DIR):
	mkdir -pv $(BUILD_DIR)

$(BUILD_DIR)/%.o: ./srcs/%.c
	$(CC) $(CFLAGS) $(INC) -c $< -o $@

clean:
	$(RM) -r $(BUILD_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all
