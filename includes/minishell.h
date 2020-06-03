/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/03 18:28:28 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/src/libft.h"
# include <signal.h>
# include <wait.h>
# include <errno.h>

# define GREEN "\033[1;32m"
# define BLUE "\033[1;36m"
# define YELLOW "\033[0;33m"
# define RED "\033[1;31m"
# define END "\033[0m"

# define SPACE      32
# define TAB        9
# define NEWLINE    10

typedef struct	s_arg{
	int			count;
	char		**value;
    int         max_arg_size;
}				t_arg;

/* minishell.c */
void	prompt(int error);

/* parsing.c */
void    parse(char *input, t_arg *arg);

/* check.c */
void    print_args(int argc, char **argv, char *str);

/* free.c */
void    free_arg(t_arg *arg);

/* init.c */
void    init_arg(t_arg *arg);

/* signal_handling.c */
void	control_slash(int num);
void	control_d();
void	control_c(int num);

#endif