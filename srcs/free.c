/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/09 19:53:44 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
free_rdir(t_rdir *rdir)
{
    if (!rdir)
        return ;
    if (rdir->next)
        free_rdir(rdir->next);
    if (rdir->value)
        free(rdir->value);
    rdir->value = NULL;
    rdir->type = 0;
    free(rdir);
    rdir = NULL;
}

    void
free_cmd(t_cmd *cmd)
{
    int i;

    if (!cmd)
        return ;
    if (cmd->next)
        free_cmd(cmd->next);
    i = 0;
    while (i < cmd->argc)
    {
        free(cmd->argv[i]);
        cmd->argv[i] = NULL;
        i++;
    }
    if (cmd->rdir)
        free_rdir(cmd->rdir);
    cmd->rdir = NULL;
    cmd->argc = 0;
    cmd->pipe = 0;
    free(cmd);
    cmd = NULL;
}