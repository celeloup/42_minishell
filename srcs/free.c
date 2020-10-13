/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/12 14:58:58 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

t_cmd *free_cmd(t_cmd *cmd)
{
	int i;
	t_cmd *tmp;
	t_rdir *tmp_rdir;

	while (cmd)
	{
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
		while (cmd->rdir)
		{
			free(cmd->rdir->value);
			tmp_rdir = cmd->rdir->next;
			free(cmd->rdir);
			cmd->rdir = tmp_rdir;
		}
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
	return (NULL);
}
/*
t_cmd *free_cmd(t_cmd *cmd)
{
	int i;
	t_cmd *tmp;
	t_rdir *tmp_rdir;

	while (cmd)
	{
		i = 0;
		while (cmd->argv && cmd->argv[i])
		{
			free(cmd->argv[i]);
			i++;
		}
		free(cmd->argv);
		while (cmd->rdir)
		{
			free(cmd->rdir->value);
			tmp_rdir = cmd->rdir->next;
			free(cmd->rdir);
			cmd->rdir = tmp_rdir;
		}
		tmp = cmd->next;
		free(cmd);
		cmd = tmp;
	}
	return (NULL);
}*/
