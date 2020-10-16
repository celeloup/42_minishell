# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2020/05/28 09:41:35 by celeloup          #+#    #+#              #
#    Updated: 2020/10/16 17:39:31 by celeloup         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME			= minishell

SRCS_FILE		= builtin_cd.c \
builtin_cd_utils.c \
builtin_echo.c \
builtin_env.c \
builtin_env_utils.c \
builtin_exit.c \
builtin_export.c \
builtin_pwd.c \
builtin_unset.c \
builtin_var_utils.c \
copy.c \
debug.c \
execution.c \
free.c \
init.c \
minishell.c \
parsing_adult_argv.c \
parsing_char.c \
parsing_give_cmd_birth.c \
parsing_make_baby_a_teen.c \
parsing_make_cmd_an_adult.c \
parsing_quote.c \
parsing_rdir.c \
parsing_utils.c \
parsing_var.c \
signal_handling.c \
bin.c \
pipe.c \
redirection.c \
exec_utils.c

SRCS			= $(addprefix $(SRCS_DIR), $(SRCS_FILE))
OBJS			= $(addprefix $(OBJS_DIR), $(SRCS_FILE:.c=.o))

SRCS_DIR		= ./srcs/
OBJS_DIR		= ./objs/
INCLUDES_DIR	= ./includes/
LIBFT_DIR		= ./libft/

LIBFT			= $(LIBFT_DIR)libft.a
HEADER			= $(INCLUDES_DIR)minishell.h
CFLAGS			= -Wall -Wextra -Werror #-g3 -fsanitize=address,undefined -Wpadded
CC				= clang

INCLUDES	= $(addprefix -I, $(INCLUDES_DIR) $(LIBFT_DIR))

# COLORS
_BLACK		= "\033[30m"
_RED		= "\033[31m"
_GREEN		= "\033[32m"
_YELLOW		= "\033[33m"
_BLUE		= "\033[34m"
_VIOLET		= "\033[35m"
_CYAN		= "\033[36m"
_WHITE		= "\033[37m"
_END		= "\033[0m"
_CLEAR		= "\033[2K"
_HIDE_CURS	= "\033[?25l"
_SHOW_CURS	= "\033[?25h"
_UP			= "\033[A"
_CUT		= "\033[k"

.PHONY: all launch clean fclean re
.SILENT:

all: launch

launch:
	echo $(_BLUE)MINISHELL PROJECT$(_END)
	$(MAKE) $(LIBFT)
	echo -n $(_CLEAR)$(_YELLOW)"building - "$(_GREEN)$(NAME)$(_END)
	$(MAKE) $(NAME)
	echo $(_GREEN)"\nDone."$(_END)

$(OBJS_DIR):
	mkdir $@

$(LIBFT): FORCE
	$(MAKE) -sC $(LIBFT_DIR)
	echo

FORCE:

$(NAME): $(OBJS_DIR) $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) $(LIBFT) -o $(NAME)

$(OBJS): $(OBJS_DIR)%.o: $(SRCS_DIR)%.c $(HEADER)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	printf $<

clean:
	$(MAKE) -sC $(LIBFT_DIR) clean
	$(RM) -r $(OBJS_DIR)

fclean: clean
	$(RM) $(LIBFT)
	$(RM) $(NAME)
	echo $(_RED)"deleted: minishell"$(_END)

re:
	$(MAKE) -s fclean
	$(MAKE) -s
