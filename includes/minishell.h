/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/25 11:47:18 by user42           ###   ########.fr       */
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

/* arg types or options for some functions */
# define RDIR_IN		1
# define RDIR_OUT		2
# define APP_RDIR_OUT	3
# define EXP			1
# define NOT_EXP		0
# define YES			1
# define NO				0
# define FT_EXIT		0
# define FT_ECHO		1
# define FT_ENV			2
# define FT_PWD			3
# define FT_CD			4
# define FT_EXPORT		5
# define FT_UNSET		6
//# define VALUE		1
//# define NO_VALUE		0


/* error types */
# define INVALID_NAME	-1
# define UNEXPECTED_TOKEN -2


typedef struct		s_env{
	char			*var;
	struct s_env	*env;
}					t_env;

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

/* parsing_utils.c */
int     is_ifs(char c);
int     is_quote(char c);
int     is_rdir(char c);
int     is_cmd_sep(char c);
int     is_arg_sep(char c);

/* parsing.c */
char*	get_env_var(char *var, char *env[]);
t_cmd	*parse_input(char *input, char *env[]);
int		len_after_char(char *input, char *env[], int quote, int expanded);
char	*expanded_str(char *input, char *env[], int quote);
char	*get_var_value(char *input, char *env[]);
char	*get_var_name(char *input);

/* check.c */
int		is_not_name(char *input);
int		is_name(char *input);
int		check_cmd_name(char *name);

/* error.c */
int		print_env_error(char *input, char *cmd, int error_type);
int		parsing_error(char *input, int error_type);

/* exit_status.c */
void	edit_exit_status(char **env[], int status);

/* debug.c */
void	print_args(int argc, char **argv);
void	print_cmd_rdir(t_rdir *rdir);
void	print_cmd(t_cmd *cmd, int i);

/* free.c */
t_cmd	*free_cmd(t_cmd *cmd_list);
void	free_rdir(t_rdir **rdir);

/* init.c */
char	**init_env(char *env[]);
t_cmd	*init_cmd();
t_rdir	*init_rdir();

/* signal_handling.c */
void	control_slash(int num);
void	control_d();
void	control_c(int num);
void	signal_handler(int num);

/* builtins.c */
int		ft_echo(t_cmd *cmd, char **env[]);
int		ft_cd(t_cmd *cmd, char **env[]);
int		ft_pwd(t_cmd *cmd, char **env[]);
int		ft_export(t_cmd *cmd, char **env[]);
int		ft_unset(t_cmd *cmd, char **env[]);


/* environment.c */
char	**env_dup(char *env[]);
void	free_env(char *env[]);
int		remove_var(char **env[], char *cmd, char *var, int value_expected);
int		add_var(char **env[], char *cmd, char *var);
int		print_env(char *env[], int option);

/* execution.c */
int		is_builtin(t_cmd *cmd, char *env[]);
int		redirections(t_rdir *rd, int type);
void	error_exit(char *actor, char *msg);
int		exec_cmd(t_cmd *cmd, char *env[]);
void	close_fd(int fd);
void	redirect_pipe(int old_fd, int new_fd);
void	exec_pipeline(t_cmd *cmd, char *env[], int in_fd);
int		exec_cmds(t_cmd *cmd, char **env[]);

#endif
