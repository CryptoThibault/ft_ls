NAME := ft_ls

CC := cc
CFLAGS := -Wall -Wextra -Werror

SRC_DIR := src
INC_DIR := include
OBJ_DIR := obj

SRCS := $(SRC_DIR)/main.c \
	$(SRC_DIR)/parse_args.c \
	$(SRC_DIR)/list_paths.c \
	$(SRC_DIR)/list_directory.c \
	$(SRC_DIR)/entry_path.c \
	$(SRC_DIR)/file_mode.c \
	$(SRC_DIR)/format_fields.c \
	$(SRC_DIR)/measure_entries.c \
	$(SRC_DIR)/print_long.c \
	$(SRC_DIR)/read_entries.c \
	$(SRC_DIR)/store_entries.c \
	$(SRC_DIR)/sort_entries.c \
	$(SRC_DIR)/print_entries.c
OBJS := $(SRCS:$(SRC_DIR)/%.c=$(OBJ_DIR)/%.o)

all: $(NAME)

$(NAME): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(NAME)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(INC_DIR)/ft_ls.h
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -I$(INC_DIR) -c $< -o $@

clean:
	$(RM) -r $(OBJ_DIR)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re
