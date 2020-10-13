/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_make_baby_a_teen.c                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/13 17:27:43 by user42           ###   ########.fr       */
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
	{
//		ft_printf("\n&input[len] =>%s<", &input[len]);//debug
		len += go_to_next_char(&input[len], NO);
	}
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

int		var_value_len(char *teen, char *baby, char *env[])
{
	int		len;
	int		i;
	char	*var_name;
	char	*var_value;

	i = 0;
	if (!var_len(baby, env))
		return (0);
	var_name = get_var_name(baby);
	var_value = get_var_value(var_name, env);
	if (((len = ft_strlen(var_value)) >= 0))
		while (var_value && var_value[i])
		{
			teen[i] = var_value[i];
			i++;
		}
	else
		len = 0;
	var_name = free_and_null_str(&var_name);
	var_value = free_and_null_str(&var_value);
	return (len);
}

int		get_teen_part(char *teen, char *baby, char *env[])
{
	int		len;

	if (baby[0] == DOLLAR && baby[ft_strlen(baby) - 1] == DOLLAR
		&& baby[1])
	{
		teen[0] = '\"';
		ft_strncpy(&teen[1], baby, var_len(baby, NULL));
		len = var_len(baby, NULL) + 2;
		teen[len - 1] = '\"';
	}
//	else if (baby[0] == DOLLAR && var_len(baby, env) == -1)
//		len = 0;
	else if (baby[0] == DOLLAR)
		len = var_value_len(teen, baby, env);
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
//		ft_printf("\nteen = >%s<", teen);//debug
//		ft_printf("\n&baby[i] = >%s<", &baby[i]);//debug
		len += get_teen_part(&teen[len], &baby[i], env);
		i += go_to_next_char(&baby[i], NO);
	}
//	ft_printf("\nteen = >%s<", teen);//debug
	return (teen);
}
