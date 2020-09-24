/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/24 17:18:49 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** "?=" is set at the last line of env to store the last exit status value
*/
char	**init_env(char *env[])
{
	int     i;
	char    **new_env;

	i = 0;
	while (env[i])
		i++;
	if ((new_env = malloc((i + 2) * sizeof(char*))) == NULL)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = ft_strdup("?=");
    new_env[i + 1] = NULL;
	return (new_env);
}

t_rdir  *init_rdir(void)
{
    t_rdir  *rdir;

    rdir = NULL;
    rdir = (t_rdir *)malloc(sizeof(t_rdir));
    if (!rdir)
        return (NULL);
    rdir->type = 0;
    rdir->value = NULL;
    rdir->next = NULL;
    return (rdir);
}

t_cmd   *init_cmd(void)
{
    t_cmd   *cmd;

    cmd = NULL;
    cmd = (t_cmd *)malloc(sizeof(t_cmd));
    if (!cmd)
        return(NULL);
    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->rdir = NULL;
    cmd->pipe = 0;
    cmd->next = NULL;
    return (cmd);
}
