/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_binaries.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/09/25 14:54:17 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		is_ifs(char c)
{
	if (c == SPACE || c == TAB || c == NEWLINE)
		return (1);
	else
		return (0);
}

int		is_quote(char c)
{
	if (c == SINGLE_QUOTE || c == DOUBLE_QUOTE)
		return (1);
	else
		return (0);
}

int		is_rdir(char c)
{
	if (c == '>' || c == '<')
		return (1);
	else
		return (0);
}

int		is_cmd_sep(char c)
{
	if (c == '|' || c == ';')
		return (1);
	else
		return (0);
}

int		is_arg_sep(char c)
{
	if (is_ifs(c) || is_rdir(c) || is_cmd_sep(c))
		return (1);
	else
		return (0);
}
