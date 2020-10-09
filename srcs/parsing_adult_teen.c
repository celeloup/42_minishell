/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_adult_teen.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/09 21:59:06 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** child_len gets the len of a child or a teen
*/

int		child_len(char *input)
{
	int len;

	len = 0;
	while (input[len] && !(is_arg_sep(input[len])))
		len += go_to_next_char(&input[len], NO);
	return (len);
}

int		teen_len(char *baby, char *env[])
{
	int	len;
	int	null_found;
	int	i;

	len = 0;
	i = 0;
	null_found = NO;
	while (baby[i])
	{
		if (baby[i] == DOLLAR && baby[ft_strlen(baby) - 1] == DOLLAR
			&& i != (int)(ft_strlen(baby) - 1))
			len += var_len(&baby[i], NULL) + 2;
		else if (baby[i] == DOLLAR && var_len(&baby[i], env) == -1)
			null_found = YES;
		else if (baby[i] == DOLLAR)
			len += var_len(&baby[i], env);
		else
			len += go_to_next_char(&baby[i], NO);
		i += go_to_next_char(&baby[i], NO);
	}
	if (null_found && !len)
		return (-1);
	return (len);
}

int		get_teen_part(char *teen, char *baby, char *env[])
{
	int		len;
	char	*var_value;

	var_value = NULL;
	if (baby[0] == DOLLAR && baby[ft_strlen(baby) - 1] == DOLLAR
		&& baby[1])
	{
		teen[0] = '\"';
		ft_strncpy(&teen[1], baby, var_len(baby, NULL));
		len = var_len(baby, NULL) + 2;
		teen[len - 1] = '\"';
	}
	else if (baby[0] == DOLLAR)
	{
		var_value = get_var_value(baby, env);
		ft_strcpy(teen, var_value);
		len = ft_strlen(var_value);
		var_value = free_and_null_str(&var_value);
	}
	else
	{
		ft_strncpy(teen, baby, go_to_next_char(baby, NO));
		len = go_to_next_char(baby, NO);
	}
	return (len);
}

char	*make_baby_a_teen(char *baby, char *env[])
{
	char	*teen;
	int		len;
	int		i;

	if ((len = teen_len(baby, env)) < 0)
		return (NULL);
	teen = (char*)malloc(sizeof(char) * (len + 1));
	teen[len] = '\0';
	i = 0;
	len = 0;
	while (teen[len] && baby[i])
	{
		len += get_teen_part(&teen[len], &baby[i], env);
		i += go_to_next_char(&baby[i], NO);
	}
	return (teen);
}
