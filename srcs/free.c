/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/08 14:41:20 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
/*
char	**free_env_or_argv(char **env[]) // remplacé par free_and_null_tab
{
	int		i;

	i = 0;
	while ((*env) && (*env)[i])
	{
		if ((*env)[i])
			free((*env)[i]);
		(*env)[i] = NULL;
		i++;
	}
	if ((*env))
		free((*env));
	(*env) = NULL;
	return (NULL);
}

*/
t_rdir	*free_and_null_rdir(t_rdir **rdir)
{
	if (!(*rdir))
		return (NULL);
	if ((*rdir)->next)
		(*rdir)->next = free_and_null_rdir(&((*rdir)->next));
	if ((*rdir)->value)
		(*rdir)->value = free_and_null_str(&((*rdir)->value));
	(*rdir)->type = 0;
	free(*rdir);
	return (NULL);
}

t_cmd	*free_and_null_cmd(t_cmd **cmd)
{
	if (!cmd || !(*cmd))
		return (NULL);
	if ((*cmd)->next)
		(*cmd)->next = free_and_null_cmd(&(*cmd)->next);
	(*cmd)->argc = 0;
	if ((*cmd)->argv)
		(*cmd)->argv = free_and_null_tab(&(*cmd)->argv);
	(*cmd)->pipe = 0;
	if ((*cmd)->rdir)
		(*cmd)->rdir = free_and_null_rdir(&(*cmd)->rdir);
	free((*cmd));
	return (NULL);
}

/*
t_cmd	*free_cmd(t_cmd *cmd)
{
	if (!cmd)
		return (NULL);
	if (cmd->next)
		cmd->next = free_cmd(cmd->next);
	if (cmd->argv)
		free_cmd_argv(cmd);
	if (cmd->rdir)
		free_rdir(&(cmd->rdir));
	cmd->argc = 0;
	cmd->pipe = 0;
	free(cmd);
	cmd = NULL;
	return (NULL);
}
*/
