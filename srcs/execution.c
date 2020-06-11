/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/11 09:41:17 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/11 10:41:38 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** is_builtin -> if cmd is a built-in and execute fonction accordingly
*/
int		is_builtins(t_cmd *cmd, char *env[])
{
	if (ft_strcmp("exit", cmd->argv[0]) == 0)
		ft_exit(cmd, env);
	else if (ft_strcmp("echo", cmd->argv[0]) == 0)
		ft_echo(cmd, env);
	else if (ft_strcmp("cd", cmd->argv[0]) == 0)
		ft_cd(cmd, env);
	else if (ft_strcmp("pwd", cmd->argv[0]) == 0)
		ft_pwd(cmd, env);
	else if (ft_strcmp("export", cmd->argv[0]) == 0)
		ft_export(cmd, env);
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		ft_unset(cmd, env);
	else if (ft_strcmp("env", cmd->argv[0]) == 0)
		ft_env(cmd, env);
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
	write(2, actor, ft_strlen(actor));
	write(2, ": ", 2);
	write(2, msg, ft_strlen(msg));
	write(2, "\n", 1);
	exit(EXIT_FAILURE);
}
/*
char	**get_bin(char *path)
{
	
}

char	*path_join(char *bin, char *path)
{
	
}
*/
/*
** exec_cmd -> execute a single command
** Apply redirection
** Check if cmd is a built-in
** If not, execute with execve with correct path
*/

int		exec_cmd(t_cmd *cmd, char *env[])
{
	if (redirections(cmd->rdir) == -1)
		error_exit("redirection", "failed.");
	if (is_builtins(cmd, env) == -1)
	{
		if (execve(cmd->argv[0], cmd->argv, env) == -1)
			error_exit(cmd->argv[0], "command not found.");
	}
	return (0);
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
** if there's no more cmd after the current one or if the current one has no pipe
** -> just redirect the previous cmd's out to current in and execute cmd
** else create a pipe and fork
** in children -> redirect the given in fd in cmd in and the cmd's out in the pipe's out then execute cmd
** in parent -> call exec_pipeline again with pipe fd
*/

void	exec_pipeline(t_cmd *cmd, char *env[], int in_fd)
{
	pid_t	pid;
	int		fd[2];

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
		}
	}
}

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

	while (cmd)
	{
		if ((pid = fork()) == -1)
			error_exit("fork", "failed."); //maybe return ici instead ?
		if (pid == 0)
		{
			if (cmd->pipe == 1)
				exec_pipeline(cmd, env, STDIN_FILENO);
			else
				exec_cmd(cmd, env);
		}
		wait(&status);
		if (cmd->pipe == 1)
			cmd = cmd->next;
		cmd = cmd->next;
	}
	return (status);
}
