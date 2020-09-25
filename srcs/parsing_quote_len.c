/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing-quote-len.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:20:14 by user42            #+#    #+#             */
/*   Updated: 2020/09/25 14:30:11 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		single_quote_len(char *input, int expanded)
{
	int len;
	int	exp_len;

	exp_len = 0;
	len = 1;
	while (input[len] && input[len] != SINGLE_QUOTE)
		len++;
	if (input[len] && input[len] == SINGLE_QUOTE)
	{
		exp_len -= 2;
		len++;
	}
	if (expanded)
		return (exp_len + len);
	return (len);
}

int		double_quote_len(char *input, char *env[], int expanded)
{
	int len;
	int	exp_len;

	exp_len = 0;
	len = 1;
	while (input[len] && input[len] != DOUBLE_QUOTE)
	{
		exp_len -= len_after_char(&input[len], env, DOUBLE_QUOTE, NOT_EXP);
		exp_len += len_after_char(&input[len], env, DOUBLE_QUOTE, EXP);
		len += len_after_char(&input[len], env, DOUBLE_QUOTE, NOT_EXP);
	}
	if (input[len] && input[len] == DOUBLE_QUOTE)
	{
		exp_len -= 2;
		len++;
	}
	if (expanded)
		return (exp_len + len);
	return (len);
}

int		quote_len(char *input, char *env[], int expanded)
{
	if (input[0] == DOUBLE_QUOTE)
		return (double_quote_len(input, env, expanded));
	else
		return (single_quote_len(input, expanded));
}
