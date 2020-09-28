/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:33:25 by user42            #+#    #+#             */
/*   Updated: 2020/09/28 11:40:34 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** returns the value of a var (input) in env
** also returns the value of $?
** input[0] == '$'
*/

char	*get_var_value(char *input, char *env[])
{
	size_t	i;

	i = 0;
	if (input && !input[1])
		return (ft_strdup("$"));
	while (input && env[i])
	{
		if (!strncmp(env[i], input + 1, ft_strlen(input + 1))
			&& env[i][ft_strlen(input + 1)]
			&& env[i][ft_strlen(input + 1)] == '=')
			return (ft_strdup(ft_strrchr(env[i], '=') + 1));
		i++;
	}
	return (ft_strdup(""));//vérifier s'il ne renvoie pas plutôt une chaine vide
}

int		var_len_not_exp(char *input)
{
	int		len;

	len = 1;
	if (input[len] && input[len] == '?')
		return (2);
	while (input[len] && ft_isalnum(input[len]))
		len++;
	return (len);
}

/*
** gets the len of a var (input) after =
** input[0] == '$'
** if (expanded && len == 1) is for "$" with nothing afterward
*/

int		var_len(char *input, char *env[], int expanded)
{
	int		len;
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	len = var_len_not_exp(input);
	if (!expanded)
		return (len);
	if (expanded && len == 1)
		return (1);
	var_name = (char *)malloc(sizeof(char) * (len + 1));
	ft_strncpy(var_name, input, len);
	var_name[len] = '\0';
	var_value = get_var_value(var_name, env);
	if (!var_value)
		return (0);
	len = ft_strlen(var_value);
	free(var_name);
	free(var_value);
	var_name = NULL;
	var_value = NULL;
	return (len);
}

char	*get_var_name(char *input)
{
	int		i;
	int		len;
	char	*name;

	len = var_len(input, NULL, NOT_EXP);
	if (len == 2 && input[1] == '?')
		return (ft_strdup("$?"));
	name = NULL;
	name = (char *)malloc(sizeof(char) * (len + 1));
	name[0] = DOLLAR;
	name[len] = '\0';
	i = 1;
	while (input[i] && ft_isalnum(input[i]))
	{
		name[i] = input[i];
		i++;
	}
	return (name);
}
