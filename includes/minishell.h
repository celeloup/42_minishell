/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/12 14:55:19 by celeloup         ###   ########.fr       */
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
# define BACKSLASH		92
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
# define AFTER			1
# define BEFORE			2

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
** builtins files
*/
void	edit_exit_status(char **env[], int status);
int		print_env_error(char *input, char *cmd, int error_type);
int		print_env(char *env[], int option);
int		ft_env(t_cmd *cmd, char **env[]);
int		var_is_set(char **env[], char *var);
int		ft_export(t_cmd *cmd, char **env[]);
int		ft_unset(t_cmd *cmd, char **env[]);
int		ft_exit(t_cmd *cmd, char **env[]);
int		ft_echo(t_cmd *cmd, char **env[]);
int		ft_cd(t_cmd *cmd, char **env[]);
int		ft_pwd(t_cmd *cmd, char **env[]);
int		env_len(char *env[]);
char	**env_dup(char *env[]);
char	**env_ncpy(char *dest[], char *src[], int start, int n);
int		var_is_valid(char *var, char *cmd, int value_expected);

/*
** copy.c
*/
char	**copy_argv(char **argv, int argc);
t_rdir	*copy_rdir(t_rdir *rdir);
t_cmd	*copy_cmd(t_cmd *cmd);

/*
** parsing files
*/
int		is_ifs(char c);
int		is_quote(char c);
int		is_rdir(char c);
int		is_cmd_sep(char c);
int		is_arg_sep(char c);
int		words_in_baby(char *baby, char *env[]);
char	*make_baby_a_teen(char *baby, char *env[]);
char	**get_adult_argv(char **baby, int adult_argc, char *env[]);
int		get_adult_rdir(t_rdir **adult, t_rdir *baby, char *env[]);
int		baby_quote_len(char *input);
char	*get_quote(char *input, char *env[]);
char	*get_var_value(char *input, char *env[]);
char	*get_var_name(char *input);
int		var_len(char *input, char *env[]);
int		go_to_next_char(char *input, int quote);
int		get_adult_part(char *adult, char *teen, char *env[], int quote);
int		backslash_len(char *input, int quote, int expanded);
char	*get_escaped_char(char *input, int quote);
int		unexpected_token_msg(char *input);
char	*get_one_adult_arg(char *teen, char *env[]);
int		child_len(char *input);
int		get_rdir_type(t_rdir *rdir, char *input);
int		get_baby_rdir(t_rdir **rdir, char *input);
t_cmd	*give_cmd_birth(char *input);

/* debug.c */
void	print_args(int argc, char **argv);
void	print_cmd_rdir(t_rdir *rdir);
void	print_cmd(t_cmd *cmd, int i);

/*
** free.c
*/
t_cmd	*free_and_null_cmd(t_cmd **cmd_list);
t_rdir	*free_and_null_rdir(t_rdir **rdir);

/*
** init.
*/
char	**init_env(char *env[]);
char	**init_argv(int argc);
t_cmd	*init_cmd();
t_rdir	*init_rdir();

/*
** expand_args.c
*/
t_cmd	*copy_cmd(t_cmd *cmd);
int		make_cmd_an_adult(t_cmd *cmd, char *env[]);

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
int		var_is_valid(char *var, char *cmd, int value_expected);
void	sort_env(char *env[]);
/*
** execution.c
*/
int		is_builtin(t_cmd *cmd, char *env[]);
int		redirections(t_rdir *rd, int type);
void	error_exit(int status, t_cmd *cmd, char *env[]);
int		exec_cmd(t_cmd *cmd, char *env[]);
void	close_fd(int fd);
void	redirect_pipe(int old_fd, int new_fd);
void	exec_pipeline(t_cmd *cmd, char *env[], int in_fd);
int		exec_cmds(t_cmd *cmd, char **env[]);

#endif
