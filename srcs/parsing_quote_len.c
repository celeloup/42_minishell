/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quote_len.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:20:14 by user42            #+#    #+#             */
/*   Updated: 2020/10/08 15:45:17 by user42           ###   ########.fr       */
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
	ft_printf("\nDOUBLE_QUOTE_LEN_EXP input debut = >%s<", input);
	while (input[len] && input[len] != DOUBLE_QUOTE)
	{
		ft_printf("\nDOUBLE_QUOTE_LEN_EXP input middl = >%s<", &input[len]);
		exp_len -= len_after_char(&input[len], env, DOUBLE_QUOTE, NOT_EXP);
		exp_len += len_after_exp_char(&input[len], env, DOUBLE_QUOTE);
		len += len_after_char(&input[len], env, DOUBLE_QUOTE, NOT_EXP);
		ft_printf("\nDOUBLE_QUOTE_LEN middl = >%d<", len);
	}
	if (input[len] && input[len] == DOUBLE_QUOTE)
	{
		exp_len -= 2;
		len++;
	}
	ft_printf("\nDOUBLE_QUOTE_LEN_NOT_EXP = %d", len);
	ft_printf("\nDOUBLE_QUOTE_LEN_EXP = %d", exp_len + len);
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
