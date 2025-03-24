NAME = philo_bonus

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3

OBJDIR = ../objects

SRCS	=	philo_bonus.c \
			utils_bonus.c \
			utils_bonus2.c \
			main_bonus.c \

OBJS = $(patsubst %.c, $(OBJDIR)/%.o, $(SRCS))

RM = rm -f

all: $(NAME)

$(NAME): $(OBJS)
	@$(CC) $(CFLAGS) -o $(NAME) $(OBJS) -lpthread
	@echo "Compiled $(NAME) successfully!"

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	@$(CC) $(CFLAGS) -c $< -o $@

clean:
	@$(RM) $(OBJS)
	@rmdir $(OBJDIR) 2>/dev/null || true
	@echo "Cleaned all object files."

fclean: clean
	@$(RM) $(NAME)
	@echo "Cleaned $(NAME) and all objects."

re: fclean all

.PHONY: all clean fclean re
