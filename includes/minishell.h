/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/05 17:48:19 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/src/libft.h"
# include <signal.h>
# include <wait.h>
# include <errno.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>

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

typedef struct		s_cmd
{
	char			**argv;
	char			**output_redirect; // >
	char			**output_append; // >>
	char			**input_redirect; // <
	int				pipe;
	int				argc;
	struct s_cmd	*next;
}					t_cmd;

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

/* builtins.c */
void	ft_exit(t_cmd *cmd, char *env[]);
void	ft_echo(t_cmd *cmd, char *env[]);
void	ft_cd(t_cmd *cmd, char *env[]);
void	ft_pwd(t_cmd *cmd, char *env[]);
void	ft_export(t_cmd *cmd, char *env[]);
void	ft_unset(t_cmd *cmd, char *env[]);
void	ft_env(t_cmd *cmd, char *env[]);

#endif