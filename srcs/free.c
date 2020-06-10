/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/09 20:52:02 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
free_rdir(t_rdir **rdir)
{
    if (!(*rdir))
        return ;
    if ((*rdir)->next)
        free_rdir(&((*rdir)->next));
    if ((*rdir)->value)
        free((*rdir)->value);
    (*rdir)->value = NULL;
    (*rdir)->type = 0;
    free(*rdir);
    (*rdir) = NULL;
}

    void
free_cmd_argv(t_cmd *cmd)
{
    int i;

    i = 0;
    while (cmd->argv[i])
    {
        free(cmd->argv[i]);
        cmd->argv[i] = NULL;
        i++;
    }
    free(cmd->argv);
    cmd->argv = NULL;
}
    
    t_cmd*
free_cmd(t_cmd *cmd)
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