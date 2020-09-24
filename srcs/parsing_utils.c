/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_utils.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/09/24 11:05:53 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"


int     is_ifs(char c)
{
    if (c == SPACE || c == TAB || c == NEWLINE)
        return (1);
    else
        return (0);
}

int     is_quote(char c)
{
    if (c == SINGLE_QUOTE || c == DOUBLE_QUOTE)
        return (1);
    else
        return (0);
}

/*
int     is_escape(char c)
{
    if (c == 'a' || c == 'b' || c == 'e' || c == 'f' || c == 'n' || c == 'r'
        || c == 't' || c == 'v' || c == BKSLASH || c == 'c' || c == 'c')
        return (1);
    else
        return (0);
}
*/
int     is_rdir(char c)
{
    if (c == '>' || c == '<')
        return (1);
    else
        return (0);
}

int     is_cmd_sep(char c)
{
    if (c == '|' || c == ';')
        return (1);
    else
        return (0);
} 

int     is_arg_sep(char c)
{
    if (is_ifs(c) || is_rdir(c) || is_cmd_sep(c))
        return (1);
    else
        return (0);
}
