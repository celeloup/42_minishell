/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 12:22:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
free_rdir(t_rdir *rdir)
{
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

    i = 0;
    if (cmd)
    {
        while (cmd->argv[i])
        {
            free(cmd->argv[i]);
            cmd->argv[i] = NULL;
            i++;
        }
        free(cmd);
        cmd = NULL;
    }
}