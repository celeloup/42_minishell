/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/01 18:30:26 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		unexpected_token_msg(char *input)
{
	if (input && input[1] && input[0] == input[1] && input[1] == '|')
		return (-1);
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!input)
		ft_putstr_fd("newline'\n", 2);
	else if (input[0])
	{
		ft_putchar_fd(input[0], 2);
		if (input[1] && input[0] == input[1])
			ft_putchar_fd(input[1], 2);
		ft_putstr_fd("'\n", 2);
	}
	return (-1);
}

int		token_len(char *input, char *env[], int expanded)
{
	int len;
	int exp_len;

	len = 0;
	exp_len = 0;
	//ft_printf("\nTOKENLEN\ninput = %s", input);//debug
	while (input[len] && !(is_arg_sep(input[len])))
	{
		exp_len -= len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
		exp_len += len_after_char(&input[len], env, NO_QUOTE, EXP);
		len += len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
	}
	if (expanded)
		return (len + exp_len);
	return (len);
}

char	*get_expanded_token(char *input, char *env[])
{
	char	*token;
	char	*str;
	int		i;
	int		j;

	token = NULL;
	str = NULL;
	i = 0;
	j = 0;
	if (!input || (input && !input[0]))
		return (NULL);
	//ft_printf("\nGETEXPTOKEN input = %s", input);//debug
	//ft_printf("\nGETEXPTOKEN tokenlen = %d", token_len(input, env, EXP));//debug
	if (!token_len(input, env, EXP) && input[0] != DOUBLE_QUOTE)
		return (NULL);
	token = (char *)malloc(sizeof(char) * (token_len(input, env, EXP) + 1));
	token[token_len(input, env, EXP)] = '\0';
	while (i < token_len(input, env, NOT_EXP) && j < token_len(input, env, EXP))
	{
		str = expanded_str(&input[i], env, NO_QUOTE);
		if (str)
		{
			ft_strcpy(&token[j], str);
			free(str);
		}
		str = NULL;
		j += len_after_char(&input[i], env, NO_QUOTE, EXP);
		i += len_after_char(&input[i], env, NO_QUOTE, NOT_EXP);
	}
	//ft_printf("\nGETEXPTOKENfin token = >%s<", token);//debug
	return (token);
}

char	*get_token(char *input, char *env[])
{
	char	*token;
	int		tkn_len;

	token = NULL;
	tkn_len = token_len(input, env, NOT_EXP);
	token = (char*)malloc(sizeof(char) * (tkn_len + 1));
	token = ft_strncpy(token, input, tkn_len);
	token[tkn_len] = '\0';
	return (token);
}
