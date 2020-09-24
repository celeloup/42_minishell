/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/23 16:17:56 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
free_rdir(t_rdir **rdir)
{
    if (!(*rdir))
        return ;
    // ft_printf("\nfreeing cmd rdir");//outil debug a supprimer
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
    // ft_printf("\nfreeing cmd argv");//outil debug a supprimer
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
    // print_cmd(cmd, 0);//outil debug a supprimer
    if (!cmd)
        return (NULL);
    // ft_printf("\nfreeing cmd");//outil debug a supprimer
    if (cmd->next)
    {
        // ft_printf("\nfreeing next cmd");//outil debug a supprimer
        cmd->next = free_cmd(cmd->next);
    }
    if (cmd->argv)
        free_cmd_argv(cmd);
    if (cmd->rdir)
        free_rdir(&(cmd->rdir));
    cmd->argc = 0;
    cmd->pipe = 0;
    free(cmd);
    cmd = NULL;
    // ft_printf("\ncmd freed");
    return (NULL);
}
