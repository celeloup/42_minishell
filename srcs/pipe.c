/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/16 17:32:52 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 17:59:26 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		pipe_child_redirect(int fdpipe[2], t_cmd *cmd, int tmpout)
{
	int ret;

	ret = 0;
	close(fdpipe[0]);
	if (cmd->pipe != 0)
	{
		if (redirection(cmd->rdir, 2, fdpipe[1], STDOUT_FILENO) == -1)
			ret = 1;
	}
	else
	{
		if (redirection(cmd->rdir, 2, tmpout, STDOUT_FILENO) == -1)
			ret = 1;
		close(fdpipe[1]);
	}
	return (ret);
}

void	pipe_child_exec(t_cmd *cmd, t_cmd *first, char **env[])
{
	int status;

	status = 0;
	if (g_builtin(cmd->argv[0]) != NULL)
	{
		status = (*g_builtin(cmd->argv[0]))(cmd, env);
		if (status != -1)
			error_exit(status, first, *env);
		else
			error_exit(get_status(env), first, *env);
	}
	else
	{
		if (!file_exist(cmd->argv[0]))
			execve(get_cmd_path(cmd->argv[0], *env), cmd->argv, *env);
		no_exec_quit(cmd->argv[0], first, env);
	}
}

void	pipeline_exec(t_cmd **cmd, char **env[], t_cmd *first, int tmpout)
{
	int fdpipe[2];

	pipe(fdpipe);
	g_var.pid = fork();
	(*cmd)->pid = g_var.pid;
	if (g_var.pid == 0)
	{
		if (pipe_child_redirect(fdpipe, *cmd, tmpout) == 1)
			error_exit(1, first, *env);
		pipe_child_exec(*cmd, first, env);
	}
	close(fdpipe[1]);
	if (redirection((*cmd)->rdir, 1, fdpipe[0], STDIN_FILENO) == -1)
		error_exit(1, first, *env);
	if ((*cmd)->pipe == 1)
	{
		*cmd = (*cmd)->next;
		while (make_cmd_an_adult(*cmd, env))
			*cmd = (*cmd)->next;
	}
}

int		pipeline(t_cmd **cmd, char **env[], t_cmd *first, int tmpout)
{
	int pipe_length;
	int i;
	int status;

	status = 0;
	pipe_length = size_pipeline(*cmd);
	i = 0;
	while (i < pipe_length)
	{
		pipeline_exec(cmd, env, first, tmpout);
		i++;
	}
	wait_for_stuff(first);
	status = (*cmd)->status;
	return (status);
}

int		size_pipeline(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->pipe == 1)
	{
		i++;
		cmd = cmd->next;
	}
	i++;
	return (i);
}
