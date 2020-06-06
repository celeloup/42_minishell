/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 12:22:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
print_args(int argc, char **argv)
{  
    int i;

    ft_printf("\nARGC = %d\nARGV = ", argc);
    i = 0;
    while (i < argc)
    {
        ft_printf("%s,", argv[i]);
        i++;
    }
}
    void
print_cmd_rd(t_rd *rd)
{
    char *type;

    type = NULL;
    if (rd->type == RD_IN)
        type = ft_strdup("<");
    if (rd->type == RD_OUT)
        type = ft_strdup(">");
    else
        type = ft_strdup(">>");
    ft_printf("%s %s ", type, rd->value);
    free(type);
    type = NULL;
    if (rd->next)
        print_cmd_rd(rd->next);
}
    
    void
print_cmd(t_cmd *cmd, int i)
{
    if (!cmd)
    {
        ft_printf("\nNO CMD LIST TO PRINT");
        return ;
    }
    ft_printf("\n----CMD%d----", i);
    ft_printf("\nARGC = %d", cmd->argc);
    
    if (cmd->argv && cmd->argc)
        print_args(cmd->argc, cmd->argv);
    if (cmd->rd)
    {
        ft_printf("\nRDIR = ");
        print_cmd_rd(cmd->rd);
    }
    ft_printf("\nPIPE = %d", cmd->pipe);
    if (cmd->next)
        print_cmd(cmd->next, i + 1);
}