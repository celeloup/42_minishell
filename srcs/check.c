/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   check.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/09 21:22:06 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

	int
check_cmd_name(char *name)
{
    int     i;
    char    *low_name;

    low_name = ft_strdup(name);
    i = 0;
    while (low_name[i])
    {
        low_name[i] = ft_tolower(low_name[i]);
        i++;
    }
    i = 0;
    if (!ft_strcmp(low_name, "echo"))
        i = ECHO;
    else if (!ft_strcmp(low_name, "env"))
        i = ENV;
    else if (!ft_strcmp(low_name, "pwd"))
        i = PWD;
    else if (!ft_strcmp(low_name, "cd"))
        i = CD;
    if (low_name)
        free(low_name);
    low_name = NULL;
    return (i);
}

    int
is_not_name(char *input)
{
    int     i;

    if (!input || (input[0] && !(ft_isalpha(input[0]) || input[0] == '_')))
        return (-1);
    i = 1;
    while(input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        i++;
    if (input[i])
        return (i);
    else
        return (0);
}

    int
is_name(char *input)
{
    int     i;

    if (!input || (input[0] && !(ft_isalpha(input[0]) || input[0] == '_')))
        return (0);
    i = 1;
    while(input[i] && (ft_isalnum(input[i]) || input[i] == '_'))
        i++;
    if (input[i])
        return (0);
    else
        return (i);
}