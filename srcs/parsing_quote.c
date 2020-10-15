/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:20:14 by user42            #+#    #+#             */
/*   Updated: 2020/10/15 19:49:15 by user42           ###   ########.fr       */
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
	int		i;
	int		len;
	char	*part;

	len = 0;
	i = 1;
	part = NULL;
	while (input[i] && input[i] != DOUBLE_QUOTE)
	{
		if (env && (part = get_adult_part(&input[i], env, DOUBLE_QUOTE)))
			len += ft_strlen(part);
		part = free_and_null_str(&part);
		i += go_to_next_char(&input[i], DOUBLE_QUOTE);
	}
	if (input[i] && input[i] == DOUBLE_QUOTE)
		i++;
	if (env)
		return (len);
	return (i);
}

int		baby_quote_len(char *input)
{
	if (input[0] == DOUBLE_QUOTE)
		return (double_quote_len(input, NULL));
	else
		return (single_quote_len(input, NO));
}

char	*get_double_quote(char *teen, char *env[])
{
	char	*quote;
	int		len;
	int		i;
	char	*part;

	if ((len = double_quote_len(teen, env)) < 0)
		return (ft_strdup(""));
	quote = (char*)malloc(sizeof(char) * (len + 1));
	quote[len] = '\0';
	i = 1;
	len = 0;
	while (teen[i] && quote[len])
	{
		if ((part = get_adult_part(&teen[i], env, DOUBLE_QUOTE)))
			len += ft_strlen(part);
		part = free_and_null_str(&part);
		i += go_to_next_char(&teen[i], DOUBLE_QUOTE);
	}
	return (quote);
}

char	*get_quote(char *teen, char *env[])
{
	if (teen[0] == SINGLE_QUOTE && single_quote_len(teen, EXP))
		return (ft_substr(teen, 1, single_quote_len(teen, EXP)));
	else if (teen[0] == SINGLE_QUOTE)
		return (ft_strdup(""));
	else
		return (get_double_quote(teen, env));
}
