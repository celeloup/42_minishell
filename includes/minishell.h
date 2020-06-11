/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/11 10:33:04 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/src/libft.h"
# include <signal.h>
# include <sys/wait.h>
# include <sys/errno.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/stat.h>

# ifndef STDIN_FILENO 
#  define STDIN_FILENO 0
# endif
# ifndef STDOUT_FILENO
#  define STDOUT_FILENO 1
# endif

# define GREEN "\033[1;32m"
# define BLUE "\033[1;36m"
# define YELLOW "\033[0;33m"
# define RED "\033[1;31m"
# define END "\033[0m"

/* some useful ASCII values */
# define SPACE      	32
# define TAB        	9
# define NEWLINE    	10
# define BKSLASH		92
# define NO_QUOTE		0
# define SINGLE_QUOTE	39
# define DOUBLE_QUOTE	34
# define DOLLAR			36

/* ASCII values combo */ /*norme ? */
# define IFS(x)			(x == SPACE || x == TAB || x == NEWLINE)
# define QUOTE(x)		(x == SINGLE_QUOTE || x == DOUBLE_QUOTE)
# define ESCAPE_CHAR(x)	(x == 'a' || x == 'b' || x == 'e' || x == 'f' || x == 'n' || x == 'r' || x == 't' || x == 'v' || x == BACKSLASH || x == 'x' || x == 'c')
# define RDIR(x)		(x == '>' || x == '<')
# define CMD_SEP(x)		(x == '|' || x == ';')
# define ARG_SEP(x)		(IFS(x) || RDIR(x) || CMD_SEP(x))

/* arg types for some functions */
# define RDIR_IN		1
# define RDIR_OUT		2
# define APP_RDIR_OUT	3
# define EXP			1
# define NOT_EXP		0

/* error types */
# define UNEXPECTED_TOKEN 1

typedef	struct		s_rdir{
	int				type;
	char			*value;
	struct s_rdir	*next;
}					t_rdir;

typedef	struct		s_cmd{
	char			**argv;
	int				argc;
	t_rdir			*rdir;
	int				pipe;
	struct s_cmd	*next;
}					t_cmd;

/* minishell.c */
void	prompt(int error);

/* parsing.c */
t_cmd	*parse_input(char *input, char *env[]);
int		len_after_char(char *input, char *env[], int quote, int expanded);
char	*expanded_str(char *input, char *env[], int quote);

/* error.c */
int		parsing_error(char *input, int error_type);

/* debug.c */
void	print_args(int argc, char **argv);
void	print_cmd_rdir(t_rdir *rdir);
void	print_cmd(t_cmd *cmd, int i);

/* free.c */
t_cmd	*free_cmd(t_cmd *cmd_list);
void	free_rdir(t_rdir **rdir);

/* init.c */
t_cmd	*init_cmd(char *input);
t_rdir	*init_rdir();

/* signal_handling.c */
void	control_slash(int num);
void	control_d();
void	control_c(int num);
void	signal_handler(int num);

/* builtins.c */
void	ft_exit(t_cmd *cmd, char *env[]);
void	ft_echo(t_cmd *cmd, char *env[]);
void	ft_cd(t_cmd *cmd, char *env[]);
void	ft_pwd(t_cmd *cmd, char *env[]);
void	ft_export(t_cmd *cmd, char *env[]);
void	ft_unset(t_cmd *cmd, char *env[]);
void	ft_env(t_cmd *cmd, char *env[]);

/* environnement.c */
char	**init_env(char *env[]);
void	free_env(char *env[]);
int		add_env(char **env[], char *var);
void	print_env(char *env[]);

/* execution.c */
int		is_builtins(t_cmd *cmd, char *env[]);
int		redirections(t_rdir *rd);
void	error_exit(char *actor, char *msg);
int		exec_cmd(t_cmd *cmd, char *env[]);
void	close_fd(int fd);
void	redirect_pipe(int old_fd, int new_fd);
void	exec_pipeline(t_cmd *cmd, char *env[], int in_fd);
int		exec_cmds(t_cmd *cmd, char *env[]);


#endif