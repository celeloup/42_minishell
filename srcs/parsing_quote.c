/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_quote.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:20:14 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 20:35:27 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*get_single_quote(char *input)
{
	char	*ret;
	int		len;

	len = single_quote_len(input, EXP);
	ret = ft_substr(input, 1, len);
	return (ret);
}

char	*get_double_quote(char *teen, char *env[])
{
	char	*quote;
	int		len;
	int		i;

	len = double_quote_len(teen, env);
	ft_printf("\ndoublequote len = %d", len);
	quote = (char*)malloc(sizeof(char) * (len + 1));
	quote[len] = '\0';
	i = 1;
	len = 0;
	while (teen[i] && quote[len])
	{
		len += get_adult_part(&quote[len], &teen[i], env, DOUBLE_QUOTE);
		ft_printf("\ndoublequote len inside = %d", len);
		i += go_to_next_char(&teen[i], DOUBLE_QUOTE);
		ft_printf("\ndoublequote i inside = %d", i);
	}
	return (quote);
}

char	*get_quote(char *teen, char *env[])
{
	if (teen[0] == SINGLE_QUOTE)
		return (get_single_quote(teen));
	else
		return (get_double_quote(teen, env));
}
