/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 12:22:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    t_rdir*
init_rdir(void)
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

    t_cmd*
init_cmd(char *input)
{
    t_cmd   *cmd;

    cmd = NULL;
    if (!input)
        return (NULL);
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