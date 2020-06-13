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