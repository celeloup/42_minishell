/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 09:41:17 by celeloup          #+#    #+#             */
/*   Updated: 2020/07/23 10:35:12 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>

/*
** is_builtin -> if cmd is a built-in and execute fonction accordingly
*/
int		is_builtin(t_cmd *cmd, char *env[])
{
	int		cmd_name;

	cmd_name = check_cmd_name(cmd->argv[0]);
	if (ft_strcmp("exit", cmd->argv[0]) == 0)
		ft_exit(cmd);
	else if (cmd_name == ECHO)
		ft_echo(cmd);
	else if (cmd_name == CD)
		ft_cd(cmd);
	else if (cmd_name == PWD)
		ft_pwd(cmd);
	else if (ft_strcmp("export", cmd->argv[0]) == 0)
		ft_export(cmd, &env);
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		ft_unset(cmd, &env);
	else if (cmd_name == ENV)
		ft_env(&env);
	else
		return (-1);
	return (0);
}

/*
** redirections -> apply all redirections to a command
** For each redirection, check type of redirection
** try to open file with corresponding flags
** try to duplicate fd into the corresponding fd (in or out)
** close inactive fd
*/

int		redirections(t_rdir *rd)
{
	int fd;

	while (rd)
	{
		if (rd->type == RDIR_IN)
		{
			fd = open(rd->value, O_RDONLY);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			if (dup2(fd, STDIN_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
		}
		else if (rd->type == RDIR_OUT)
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
		}
		else
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				error_exit(rd->value, strerror(errno));
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
		}
		rd = rd->next;
	}
	return (0);
}

void	error_exit(char *actor, char *msg)
{
	write(2, "minishell: ", 11);
	write(2, actor, ft_strlen(actor));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}

void	path_join(char *bin, char **path)
{
	int		bin_len;
	int		path_len;
	char	*new_path;
	int		i;

	bin_len = ft_strlen(bin);
	path_len = ft_strlen(*path);
	if ((new_path = malloc(bin_len + path_len + 2)) == NULL)
		return ;
	i = 0;
	while (i < bin_len)
	{
		new_path[i] = bin[i];
		i++;
	}
	new_path[i] = '/';
	i++;
	while (i < path_len + bin_len + 1)
	{
		new_path[i] = **path;
		(*path)++;
		i++;
	}
	new_path[i] = '\0';
	free(path);
	*path = new_path;
}

void	get_cmd_path(char **cmd, char *env[])
{
	char			*path;
	char			**bin_tab;
	int				i;
	DIR				*dir;
	struct dirent	*entry;

	if (*cmd[0] == '/' || !(ft_strncmp(*cmd, "./", 2)))
		return ;
	path = get_var_value("$PATH", env);
	bin_tab = ft_split(path, ':');
	free(path);
	i = 0;
	while (bin_tab[i])
	{
		if ((dir = opendir(bin_tab[i])) != NULL)
		{
			while ((entry = readdir(dir)))
			{
				if (!ft_strcmp(entry->d_name, *cmd))
				{
					closedir(dir);
					path_join(bin_tab[i], cmd);
					free_tab(bin_tab);
					return ;
				}
			}
			closedir(dir);
		}
		i++;
	}
	free_tab(bin_tab);
}

/*
** exec_cmd -> execute a single command
** Apply redirection
** Check if cmd is a built-in
** If not, execute with execve with correct path
*/

int		exec_cmd(t_cmd *cmd, char *env[])
{
	int retour;

	retour = 0;
	if (redirections(cmd->rdir) == -1)
		error_exit("redirection", "failed.");
	if (is_builtin(cmd, env) == -1)
	{
		if (execve(cmd->argv[0], cmd->argv, env) == -1)
			error_exit(cmd->argv[0], "command not found");
	}
	return (retour);
}

/*
** close_fd -> close a give fd
*/

void	close_fd(int fd)
{
	if (close(fd) == -1)
		error_exit("close", "failed.");
}

/*
** redirect_pipe -> redirect an old fd to a new one
*/

void	redirect_pipe(int old_fd, int new_fd)
{
	if (old_fd != new_fd)
	{
		if (dup2(old_fd, new_fd) != -1)
			close_fd(old_fd);
		else
			error_exit("dup2", "failed.");
	}
}

/*
** exec_pipeline -> loops through a pipeline of cmd (recursive)
** if there's no more cmd after the current one or if the current one has no 
** pipe
** -> just redirect the previous cmd's out to current in and execute cmd
** else create a pipe and fork
** in children -> redirect the given in fd in cmd in and the cmd's out in the 
** pipe's out then execute cmd
** in parent -> call exec_pipeline again with pipe fd
*/

void	exec_pipeline(t_cmd *cmd, char *env[], int in_fd)
{
	pid_t	pid;
	int		fd[2];
	int		status;

	if (cmd->next == NULL || cmd->pipe == 0)
	{
		redirect_pipe(in_fd, STDIN_FILENO);
		exec_cmd(cmd, env);
		error_exit("execve", "failed.");
	}
	else
	{
		if (pipe(fd) == -1)
			error_exit("pipe", "failed.");
		if ((pid = fork()) == -1)
			error_exit("fork", "failed.");
		else if (pid == 0)
		{
			close_fd(fd[0]);
			redirect_pipe(in_fd, STDIN_FILENO);
			redirect_pipe(fd[1], STDOUT_FILENO);
			exec_cmd(cmd, env);
			error_exit("execve", "failed.");
		}
		else
		{
			close_fd(fd[1]);
			close_fd(in_fd);
			exec_pipeline(cmd->next, env, fd[0]);
			waitpid(pid, &status, 0);
		}
		waitpid(pid, &status, 0);
	}
}
/*
void	pipeline(t_cmd *cmd, char *env[]) //a proteger
{
	int fd[2];
	pid_t pid;
	int fdd = 0;
	int status;

	while (cmd != NULL) {
		//ft_printf("piping cmd %s \n", cmd->argv[0]);
		pipe(fd);
		if ((pid = fork()) == -1) {
			error_exit("fork", "failed");
			exit(1);
		}
		else if (pid == 0) {
			dup2(fdd, 0);
			if (cmd->next != NULL) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			exec_cmd(cmd, env);
			exit(1);
		}
		else {
			waitpid(pid, &status, 0);
			kill(pid, SIGTERM);
			close(fd[1]);
			fdd = fd[0];
			if (cmd->pipe)
				cmd = cmd->next;
			else
				cmd = NULL;
		}
	}
	//ft_printf("end of pipeline\n");
}
*/

/*
int		modify_var(char **env[], char *var)
{
	(*env)[0] = ft_strdup(var);
	return 0;
}

int		modify_env(t_cmd *cmd, char **env[])
{
	if (redirections(cmd->rdir) == -1)
		return(-1);
	modify_var(env, cmd->argv[1]);
	write(1, "modified env !", 14);
	return (0);
}
*/

void		pipeline(t_cmd *cmd, char *env[])
{
	int saved_stdout = dup(1);
	if (saved_stdout == -1)
		write(1, "bad stdout dup", 14);
	int	saved_stdin = dup(0);
	if (saved_stdin == -1)
		write(1, "bad stdin dup", 13);
	int fdin;
	fdin = dup(saved_stdin);
	if (fdin == -1)
		write(1, "bad fdin dup", 12);
	int ret;
	int fdout;
	while (1)
	{
		dup2(fdin, 0);
		if (fdin == -1)
			write(1, "bad fdin dup 2", 14);
		close(fdin);
		
		if (cmd->next == NULL)
			fdout = dup(saved_stdout);
		else
		{
			int fdpipe[2];
			pipe(fdpipe);
			fdout = fdpipe[1];
			fdin = fdpipe[0];
		}
		dup2(fdout, 1);
		if (fdin == -1)
			write(1, "bad fdout dup 2", 15);
		close(fdout);
		ret = fork();
		if (ret == 0)
		{
			exec_cmd(cmd, env);
		}
		if (cmd->pipe)
			cmd = cmd->next;
		else
			break;
	}
	dup2(saved_stdin, 0);
	dup2(saved_stdout, 1);
	close(saved_stdin);
	close(saved_stdout);
	waitpid(ret, NULL, 0);
}
/*
void	pipeline(t_cmd *cmd, char *env[]) //a proteger
{
	int fd[2];
	pid_t pid;
	int fdd = 0;
	int status;

	while (cmd != NULL) {
		ft_printf("piping cmd %s \n", cmd->argv[0]);
		pipe(fd);
		if ((pid = fork()) == -1) {
			error_exit("fork", "failed");
			exit(1);
		}
		else if (pid == 0) {
			dup2(fdd, 0);
			if (cmd->next != NULL) {
				dup2(fd[1], 1);
			}
			close(fd[0]);
			exec_cmd(cmd, env);
			exit(1);
		}
		else {
			waitpid(pid, &status, 0);
			kill(pid, SIGTERM);
			close(fd[1]);
			fdd = fd[0];
			if (cmd->pipe)
				cmd = cmd->next;
			else
				cmd = NULL;
		}
	}
	ft_printf("end of pipeline\n");
}
*/
/*
** exec_cmds -> loops through each command and execute it
** Create a child process to execute cmd,
** Check if cmd has a pipe -> to exec_pipeline
** Else -> to exec_cmd
** Wait for child process to finish
*/

int		exec_cmds(t_cmd *cmd, char *env[])
{
	pid_t	pid;
	int		status;
	int saved_stdout;
	int saved_stdin;

	while (cmd)
	{
		// duplicate stdin et stdout
		saved_stdin = dup(0);
		saved_stdout = dup(1);
		// catch if it's a function that modifies env = export, unset, cd ?
		/*if (cmd->argv[0] && ft_strcmp("export", cmd->argv[0]) == 0)
		{
			modify_env(cmd, env);
		}
		else
		{*/
			if (cmd->pipe == 1)
					pipeline(cmd, env);
			else
			{
				if ((pid = fork()) == -1)
					error_exit("fork", "failed."); //maybe return ici instead ?
				if (pid > 0)
				{
					waitpid(pid, &status, 0);
					kill(pid, SIGTERM);
				}
				else
				{
					//if (cmd->pipe == 1)
					//	exec_pipeline(cmd, *env, STDIN_FILENO);
					//else
						exec_cmd(cmd, env);
				}
			}
		//}
		while (cmd->next && cmd->pipe == 1)
				cmd = cmd->next;
		cmd = cmd->next;
		/*
		dup2(saved_stdout, 1);
		close(saved_stdout);
		dup2(saved_stdin, 0);
		close(saved_stdin);*/
	}	
	//return (0);
	//return (status);
	return (WEXITSTATUS(status));
}
