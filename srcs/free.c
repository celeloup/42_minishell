/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/13 12:11:59 by user42           ###   ########.fr       */
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
