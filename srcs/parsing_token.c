/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_token.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/07 20:38:48 by user42           ###   ########.fr       */
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

int		get_edges(char *input, char *env[], int i)
{
	int		edges;
	char	*next_str;

	next_str = NULL;
	edges = NO;
	if (!input || !input[0] || input[0] != DOLLAR)
		return (NO);
	if (i)
		edges = BEFORE;
	ft_printf("\nIN EDGES i = >%d< &input[i] = >%s<", i, &input[i]);
	i += len_after_char(&input[i], env, NO, NOT_EXP);
	ft_printf("\nIN EDGES i = >%d<", i);
	if (!input || !input[i] || is_ifs(input[i]))
		return (edges);
	if (input[i] && input[i] != '$' && !is_ifs(input[i]))
		return (edges + AFTER);
	next_str = expanded_str(&input[i], env, NO_QUOTE, BEFORE);
	if (next_str[0] && !is_ifs(next_str[0]))
		edges += AFTER;
	next_str = free_and_null(&next_str);	
	while (input[i] && (next_str = expanded_str(&input[i], env, NO_QUOTE, BEFORE)))
	{
		if ((input[i] && input[i] == '$') && (!input[i + 1] || is_ifs(input[i + 1])))
			edges = DOLLAR;
		next_str = free_and_null(&next_str);
		i += len_after_char(&input[i], env, NO, NOT_EXP);
	}
	next_str = free_and_null(&next_str);
	return (edges);
}

int		token_len(char *input, char *env[], int expanded)
{
	int len;
	int exp_len;
	int	edges;

	len = 0;
	exp_len = 0;
	edges = NO;
	if (expanded)
		edges = get_edges(&input[len], env, len);
	ft_printf("\nTOKENLEN input = >%s<", input);//debug
	while (input[len] && !(is_arg_sep(input[len])))
	{
		exp_len -= len_after_char(&input[len], env, NO_QUOTE, NOT_EXP);
		exp_len += len_after_exp_char(&input[len], env, NO_QUOTE, edges);
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
	int		len;
	int		i;
	int		j;
	int		edges;

	token = NULL;
	str = NULL;
	i = 0;
	j = 0;
	len = 0;
	edges = NO;
//	if (!input || (input && !input[0]))
//		return (NULL);
//	if (!(len = token_len(input, env, EXP)) && input[0] != DOUBLE_QUOTE)
//		return (NULL);
	//ft_printf("\ntoken_len is : %d", len);
	len = token_len(input, env, EXP);
	ft_printf("\ntoken_len EXP is : %d\n", len);
	token = (char*)malloc(sizeof(char) * (len + 1));
	token[len] = '\0';
	while (i < token_len(input, env, NOT_EXP) && j < len)
	{
		if (edges != DOLLAR)
			edges = get_edges(input, env, i);
		str = expanded_str(&input[i], env, NO_QUOTE, edges);
		ft_printf("\nexpanded str is : >%s<", str);
		if (str)
		{
			ft_printf("\nJ index is : >%d<", j);
			ft_strcpy(&token[j], str);
			ft_printf("\ntoken middle is : >%s<\n", token);
			str = free_and_null(&str);
		}
		j += len_after_exp_char(&input[i], env, NO_QUOTE, edges);
		i += len_after_char(&input[i], env, NO_QUOTE, NOT_EXP);
	}
	str = free_and_null(&str);
	ft_printf("\ntoken end is : >%s<\n", token);
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
