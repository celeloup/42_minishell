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
print_args(int argc, char **argv, char *str)
{  
    int i;

    ft_printf("\n%sshell ARGC is = %d\nARGV is/are :\n", str, argc);
    i = 0;
    while (i < argc)
    {
        ft_printf("->%s<\n", argv[i]);
        i++;
    }
}

    void
print_cmd(t_cmd *list)
{
    print_args(list->argc, list->argv, "Mini");
    if (list->next)
        print_cmd(list->next);
}