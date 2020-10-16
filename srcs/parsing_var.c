/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:33:25 by user42            #+#    #+#             */
/*   Updated: 2020/10/16 19:06:39 by user42           ###   ########.fr       */
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
		if (!strncmp(env[i], name + 1, ft_strlen(name + 1)))
		{
			if ((env[i][ft_strlen(name + 1)]
				&& env[i][ft_strlen(name + 1)] == '=')
				&& env[i][ft_strlen(name + 1) + 1])
				return (ft_strdup(ft_strchr(env[i], '=') + 1));
			else if (!(env[i][ft_strlen(name + 1)])
				|| env[i][ft_strlen(name + 1)] == '=')
				return (ft_strdup(""));
		}
		i++;
	}
	return (NULL);
}

/*
** gets the len of a var (input) after =
** input[0] == '$'
** if (len == 1) is for "$" with nothing afterward
*/

int		var_len(char *input, char *env[])
{
	int		len;
	char	*var_name;
	char	*var_value;

	len = 1;
	if (input[1] && ft_isdigit(input[1]) && !env)
		return (2);
	else if (input[1] && ft_isdigit(input[1]))
		return (0);
	while (input[len] && input[1] && !ft_isdigit(input[1])
		&& (ft_isalnum(input[len]) || input[len] == '_'))
		len++;
	if (len == 1 && input[len] && input[len] == '?')
		len++;
	if (!env || (len == 1 && (!input[1] || !is_quote(input[1]))))
		return (len);
	else if (len == 1)
		return (0);
	var_name = ft_substr(input, 0, len);
	var_value = get_var_value(var_name, env);
	var_name = free_and_null_str(&var_name);
	len = ft_strlen(var_value);
	var_value = free_and_null_str(&var_value);
	return (len);
}

char	*get_var_name(char *input)
{
	char	*name;

	name = NULL;
	if (!(name = ft_substr(input, 0, var_len(input, NULL))))
		name = ft_strdup("");
	return (name);
}
