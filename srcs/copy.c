/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   copy.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/09 14:55:45 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 14:57:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	**copy_argv(char **argv, int argc)
{
	char	**copy;
	int		i;

	copy = (char **)malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (i < argc)
	{
		copy[i] = ft_strdup(argv[i]);
		i++;
	}
	copy[i] = NULL;
	return (copy);
}

t_rdir	*copy_rdir(t_rdir *rdir)
{
	t_rdir	*copy;

	if (!rdir)
		return (NULL);
	copy = init_rdir();
	copy->next = copy_rdir(rdir->next);
	copy->type = rdir->type;
	copy->value = ft_strdup(rdir->value);
	return (copy);
}

t_cmd	*copy_cmd(t_cmd *cmd)
{
	t_cmd	*copy;

	if (!cmd)
		return (NULL);
	copy = init_cmd();
	copy->argc = cmd->argc;
	copy->argv = copy_argv(cmd->argv, cmd->argc);
	copy->pipe = cmd->pipe;
	copy->rdir = copy_rdir(cmd->rdir);
	copy->next = copy_cmd(cmd->next);
	return (copy);
}
