NAME = philo

CC = cc
CFLAGS = -Wall -Wextra -Werror -g3 

OBJDIR = ../objects

SRCS	=	philo.c \
			utils.c \
			verif.c \
			utils2.c \

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
	@if [ -d "$(OBJDIR)" ]; then \
		echo "\033[33mCleaning object files...\033[0m"; \
		$(RM) -r $(OBJDIR); \
		echo "\033[32mClean completed successfully!\033[0m"; \
	else \
		echo "\033[33mNo objects to clean.\033[0m"; \
	fi

fclean: clean
	@if [ -f "$(NAME)" ]; then \
		echo "\033[33mRemoving $(NAME)...\033[0m"; \
		$(RM) $(NAME); \
		echo "\033[32mFull clean completed successfully!\033[0m"; \
	else \
		echo "\033[33mNo executable to clean.\033[0m"; \
	fi

re: fclean all

.PHONY: all clean fclean re
