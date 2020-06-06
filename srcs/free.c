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
free_cmd_list(t_cmd *cmd)
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