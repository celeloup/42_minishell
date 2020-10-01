/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/01 20:25:45 by user42           ###   ########.fr       */
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

/*
** some useful ASCII values
*/
# define SPACE      	32
# define TAB        	9
# define NEWLINE    	10
# define BKSLASH		92
# define NO_QUOTE		0
# define SINGLE_QUOTE	39
# define DOUBLE_QUOTE	34
# define DOLLAR			36

/*
** arg types or options for some functions
*/
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
# define INVALID_NAME	-1
# define UNEXPECTED_TOKEN -2

typedef struct		s_env{
	char			*var;
	struct s_env	*env;
}					t_env;

typedef	struct		s_rdir{
	int				type;
	char			padding[4];
	char			*value;
	struct s_rdir	*next;
}					t_rdir;

typedef	struct		s_cmd{
	char			**argv;
	int				argc;
	int				pipe;
	t_rdir			*rdir;
	struct s_cmd	*next;
}					t_cmd;

/*
** minishell.c
*/
void	prompt(int error);

/*
** parsing files
*/
int		is_ifs(char c);
int		is_quote(char c);
int		is_rdir(char c);
int		is_cmd_sep(char c);
int		is_arg_sep(char c);
int		single_quote_len(char *input, int expanded);
int		double_quote_len(char *input, char *env[], int expanded);
int		quote_len(char *input, char *env[], int expanded);
char	*get_quote(char *input, char *env[]);
char	*get_var_value(char *input, char *env[]);
char	*get_var_name(char *input);
int		var_len_not_exp(char *input);
int		var_len(char *input, char *env[], int expanded);
int		len_after_char(char *input, char *env[], int quote, int expanded);
char	*expanded_str(char *input, char *env[], int quote);
int		backslash_len(char *input, int quote, int expanded);
char	*get_escaped_char(char *input, int quote);
int		unexpected_token_msg(char *input);
int		token_len(char *input, char *env[], int expanded);
char	*get_expanded_token(char *input, char *env[]);
char	*get_token(char *input, char *env[]);
int		get_rdir_type(t_rdir *rdir, char *input);
int		get_cmd_rdir(t_rdir **rdir, char *input, char *env[]);
t_cmd	*parse_input(char *input, char *env[]);

/* debug.c */
void	print_args(int argc, char **argv);
void	print_cmd_rdir(t_rdir *rdir);
void	print_cmd(t_cmd *cmd, int i);

/*
** free.c
*/
t_cmd	*free_cmd(t_cmd *cmd_list);
void	free_rdir(t_rdir **rdir);
void	free_env(char *env[]);

/*
** init.
*/
char	**init_env(char *env[]);
char	**init_argv(int argc);
t_cmd	*init_cmd();
t_rdir	*init_rdir();

/*
** get_var_in_cmd.c
*/
void	get_var_in_cmd(t_cmd *cmd, char *env[]);

/*
** signal_handling.c
*/
void	control_slash(int num);
void	control_d();
void	control_c(int num);
void	signal_handler(int num);

/*
** builtins files
*/
void	edit_exit_status(char **env[], int status);
int		print_env_error(char *input, char *cmd, int error_type);
int		print_env(char *env[], int option);
int		ft_env(t_cmd *cmd, char **env[]);
int		ft_export(t_cmd *cmd, char **env[]);
int		ft_unset(t_cmd *cmd, char **env[]);
int		ft_exit(t_cmd *cmd, char **env[]);
int		ft_echo(t_cmd *cmd, char **env[]);
int		ft_cd(t_cmd *cmd, char **env[]);
int		ft_pwd(t_cmd *cmd, char **env[]);
int		env_len(char *env[]);
char	**env_dup(char *env[]);
char	**env_ncpy(char *dest[], char *src[], int start, int n);
int		var_is_valid(char *env[], char *var, char *cmd, int value_expected);

/*
** execution.c
*/
int		is_builtin(t_cmd *cmd, char *env[]);
int		redirections(t_rdir *rd, int type);
void	error_exit(char *actor, char *msg);
int		exec_cmd(t_cmd *cmd, char *env[]);
void	close_fd(int fd);
void	redirect_pipe(int old_fd, int new_fd);
void	exec_pipeline(t_cmd *cmd, char *env[], int in_fd);
int		exec_cmds(t_cmd *cmd, char **env[]);

#endif
