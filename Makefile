GREEN = \033[0;92m

NAME = pipex
CC = cc
CFLAGS = -g -Wall -Wextra -Werror
DFLAGS = -MMD -MP -I include

SRCDIR = src
OBJDIR = obj
LIBFT_DIR = libft
LIBFT_A = $(LIBFT_DIR)/libft.a

SRCS = $(addprefix $(SRCDIR)/,\
		main.c \
		utils.c \
		path.c \
)
OBJS = $(SRCS:$(SRCDIR)/%.c=$(OBJDIR)/%.o)
DEPS = $(OBJS:.o=.d)

all: $(NAME)

$(LIBFT_A):
		make -s -C $(LIBFT_DIR)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
		mkdir -p $(@D)
		@$(CC) $(CFLAGS) $(DFLAGS) -c $< -o $@

$(NAME): $(OBJS) $(LIBFT_A)
		@$(CC) $(CFLAGS) $(OBJS) $(LIBFT_A) -o $(NAME)
		@echo "$(GREEN) Compiled!$(DEF_COLOR)"

clean:
		rm -rf $(OBJDIR)
		make clean -s -C $(LIBFT_DIR)

fclean: clean
		rm -rf $(NAME)
		make fclean -s -C $(LIBFT_DIR)

re: fclean all

.PHONY: all clean fclean re
.SECONDARY:$(OBJS)
-include $(DEPS)
