/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 19:38:09 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		(*g_builtin(char *cmd_name))(t_cmd*, char***)
{
	if (ft_strcmp("exit", cmd_name) == 0)
		return (ft_exit);
	else if (ft_strcmp("echo", cmd_name) == 0)
		return (ft_echo);
	else if (ft_strcmp("cd", cmd_name) == 0)
		return (ft_cd);
	else if (ft_strcmp("pwd", cmd_name) == 0)
		return (ft_pwd);
	else if (ft_strcmp("export", cmd_name) == 0)
		return (ft_export);
	else if (ft_strcmp("unset", cmd_name) == 0)
		return (ft_unset);
	else if (ft_strcmp("env", cmd_name) == 0)
		return (ft_env);
	else
		return (NULL);
}

int		exec_single_cmd(t_cmd *cmd, char **env[], t_cmd *first)
{
	int status;

	status = 0;
	if (redirection(cmd->rdir, 3, -1, -1) == -1)
		status = 1;
	if (status != 1 && cmd->argv && cmd->argv[0]
		&& cmd->argv[0][0] && (g_var.pid = fork()) == 0)
	{
		if (!file_exist(cmd->argv[0]))
			execve(get_cmd_path(cmd->argv[0], *env), cmd->argv, *env);
		no_exec_quit(cmd->argv[0], first, env);
	}
	else
		waitpid(g_var.pid, &status, 0);
	return (status);
}

int		execution(t_cmd **cmd, char **env[], t_cmd *first, int tmpout)
{
	int status;

	status = 0;
	if ((*cmd) && (*cmd)->argv && (*cmd)->argv[0]
		&& g_builtin((*cmd)->argv[0]) != NULL && (*cmd)->pipe == 0)
	{
		if (redirection((*cmd)->rdir, 3, -1, -1) != -1)
			status = (*g_builtin((*cmd)->argv[0]))(*cmd, env);
		else
			status = 1;
	}
	else if (*cmd)
	{
		if ((*cmd)->pipe == 1)
			status = pipeline(cmd, env, first, tmpout);
		else
			status = exec_single_cmd(*cmd, env, first);
	}
	return (status);
}

int		exec_cmds(t_cmd *cmd, char **env[])
{
	int		status;
	int		tmpin;
	int		tmpout;
	t_cmd	*first;

	first = cmd;
	while (cmd)
	{
		status = 0;
		while (cmd && (status = make_cmd_an_adult(cmd, env)))
			cmd = cmd->next;
		if (cmd)
		{
			tmpin = dup(STDIN_FILENO);
			tmpout = dup(STDOUT_FILENO);
			status = execution(&cmd, env, first, tmpout);
			status = ((status != 1 && status != 2) 
				? WEXITSTATUS(status) : status);
			if (cmd)
				cmd = cmd->next;
			if (status != 255)
				edit_exit_status(env, status);
			redirection(NULL, 0, tmpin, STDIN_FILENO);
			redirection(NULL, 0, tmpout, STDOUT_FILENO);
		}
		if (g_var.sigint || g_var.sigquit)
			return (g_var.status);
	}
	return (status);
}
