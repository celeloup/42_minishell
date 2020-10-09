/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:33:25 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 21:38:14 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** returns the value of a var (input) in env
** also returns the value of $?
** input[0] == '$'
*/

char	*get_var_value(char *name, char *env[])
{
	size_t	i;

	i = 0;
	if (name && !name[0])
		return (ft_strdup(""));
	if (name && !name[1])
		return (ft_strdup("$"));
	while (name && env[i])
	{
		if (!strncmp(env[i], name + 1, ft_strlen(name + 1))
			&& env[i][ft_strlen(name + 1)]
			&& env[i][ft_strlen(name + 1)] == '=')
			return (ft_strdup(ft_strchr(env[i], '=') + 1));
		i++;
	}
	return (NULL);
}

int		var_len_not_exp(char *input)
{
	int		len;

	len = 1;
	if (input[len] && input[len] == '?')
		return (2);
	while (input[len] && (ft_isalnum(input[len]) || input[len] == '_'))
		len++;
	return (len);
}

/*
** gets the len of a var (input) after =
** input[0] == '$'
** if (len == 1) is for "$" with nothing afterward
*/

int		var_len_exp(char *input, char *env[])
{
	int		len;
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	len = var_len_not_exp(input);
	if (len == 1 && (!input[1] || is_ifs(input[1])))
		return (1);
	else if (len == 1)
		return (0);
	var_name = (char*)malloc(sizeof(char) * (len + 1));
	ft_strncpy(var_name, input, len);
	var_name[len] = '\0';
	var_value = get_var_value(var_name, env);
	var_name = free_and_null_str(&var_name);
	len = ft_strlen(var_value);
	var_value = free_and_null_str(&var_value);
	return (len);
}

char	*get_var_name(char *input)
{
	int		len;
	char	*name;

	name = NULL;
	if ((len = var_len_not_exp(input)) == 0)
		return (ft_strdup(""));
	if (len == 2 && input[1] == '?')
		return (ft_strdup("$?"));
	name = (char *)malloc(sizeof(char) * (len + 1));
	name[0] = DOLLAR;
	name[len] = '\0';
	len = 1;
	while (input[len] && ft_isalnum(input[len]))
	{
		name[len] = input[len];
		len++;
	}
	return (name);
}
