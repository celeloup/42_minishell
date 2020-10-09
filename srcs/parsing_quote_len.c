/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quote_len.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:20:14 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 20:35:27 by user42           ###   ########.fr       */
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

int		double_quote_len(char *input, char *env[])
{
	int i;
	int	len;

	len = 0;
	i = 1;
	while (input[i] && input[i] != DOUBLE_QUOTE)
	{
		if (env)
			len += get_adult_part(NULL, &input[i], env, DOUBLE_QUOTE);
		i += go_to_next_char(&input[i], DOUBLE_QUOTE);
	}
	if (input[i] && input[i] == DOUBLE_QUOTE)
		i++;
	if (env)
		return (len);
	return (i);
}

int		quote_len(char *input)
{
	if (input[0] == DOUBLE_QUOTE)
		return (double_quote_len(input, NULL));
	else
		return (single_quote_len(input, NO));
}
