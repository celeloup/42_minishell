/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_char.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/08 18:53:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_escaped_char(char *input, int quote)
{
	char	*ret;
	int		len;

	ret = NULL;
	if (!(len = backslash_len(input, quote, EXP)))
		return (NULL);
	if (input[1] && input[1] == NEWLINE)
		return (NULL);
	ret = (char*)malloc(sizeof(char) * (len + 1));
	ret[len] = '\0';
	if (!input[1])
		ret[0] = BACKSLASH;
	else if (quote == DOUBLE_QUOTE && !(input[1] == DOLLAR
		|| input[1] == DOUBLE_QUOTE || input[1] == BACKSLASH))
	{
		ret[0] = BACKSLASH;
		ret[1] = input[1];
	}
	else
		ret[0] = input[1];
	return (ret);
}

int		backslash_len(char *input, int quote, int expanded)
{
	if (input[0] != BACKSLASH)
		return (0);
	if (!input[1])
		return (1);
	else if (input[1] == NEWLINE && expanded)
		return (0);
	else if (!expanded || (quote == DOUBLE_QUOTE && !(input[1] == DOLLAR
		|| input[1] == DOUBLE_QUOTE || input[1] == BACKSLASH)))
		return (2);
	else
		return (1);
}

int		len_after_exp_char(char *input, char *env[], int quote)
{
	int	i;

	i = 0;
	if (is_quote(input[0]) && !quote)
		return (quote_len(input, env, EXP));
	else if (input[0] == BACKSLASH)
		return (backslash_len(input, quote, EXP));
	else if (input[0] == DOLLAR && env && quote && var_len_exp(input, env) >= 0)
		return (var_len_exp(input, env));
	else if (input[0] == DOLLAR && env && quote)
		return (0);
	while (input[i] && !is_quote(input[i]) && (!(is_arg_sep(input[i])) || quote)
		&& input[i] != BACKSLASH && (input[i] != DOLLAR || !quote))
		i++;
	if (i == 0)
		return (1);
	else
		return (i);
}

int		len_after_char(char *input, char *env[], int quote, int expanded)
{
	int	i;

	i = 0;
	if (!input)
		return (0);
	if (expanded)
		return (len_after_exp_char(input, env, quote));
	else if (is_quote(input[0]) && !quote)
		return (quote_len(input, NULL, NOT_EXP));
	else if (input[0] == BACKSLASH)
		return (backslash_len(input, quote, NOT_EXP));
	else if (input[0] == DOLLAR)
		return (var_len_not_exp(input));
	while (input[i] && !is_quote(input[i]) && (!(is_arg_sep(input[i])) || quote)
		&& input[i] != BACKSLASH && input[i] != DOLLAR)
		i++;
//	if (input[i] && quote && input[i] == quote)
//		i++;
	if (i)
		return (i);
	else
		return (1);
}
