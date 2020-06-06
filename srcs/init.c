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

    t_cmd*
init_cmd(char *input)
{
    t_cmd   *cmd;

    cmd = NULL;
    if (!input)
        return (NULL);
    cmd = (t_cmd *)malloc(sizeof(t_cmd *));
    if (!cmd)
        return(NULL);
    cmd->argv = NULL;
    cmd->argc = 0;
    cmd->rd = NULL;
    cmd->pipe = 0;
    cmd->next = NULL;
    return (cmd);
}