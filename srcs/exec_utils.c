/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/16 17:38:53 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 17:39:11 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	error_msg(char *actor, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(actor, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
}

void	error_exit(int status, t_cmd *cmd, char *env[])
{
	free_and_null_cmd(&cmd);
	free_and_null_tab(&env);
	exit(status);
}

void	wait_for_stuff(t_cmd *cmd)
{
	while (cmd)
	{
		waitpid(cmd->pid, &cmd->status, 0);
		cmd = cmd->next;
	}
}

int		get_status(char **env[])
{
	char	*tmp;
	int		status;

	tmp = get_var_value("$?", *env);
	status = atoi(tmp);
	tmp = free_and_null_str(&tmp);
	return (status);
}

void	no_exec_quit(char *cmd, t_cmd *cmd_list, char **env[])
{
	if (ft_strncmp(cmd, "./", 2) == 0 && file_exist(&cmd[2]))
	{
		error_msg(cmd, "Permission denied");
		error_exit(126, cmd_list, *env);
	}
	else if (ft_strncmp(cmd, "./", 2) == 0 || !var_is_set(env, "PATH"))
	{
		error_msg(cmd, "No such file or directory");
		error_exit(127, cmd_list, *env);
	}
	else
	{
		error_msg(cmd, "command not found");
		error_exit(127, cmd_list, *env);
	}
}
