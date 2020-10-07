/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:33:25 by user42            #+#    #+#             */
/*   Updated: 2020/10/07 22:23:28 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*word_split(char *input, int edges)
{
	char	*value;
	char	*ret;
	int		i;
	int		len;

	len = 0;
	value = NULL;
	if (edges >= BEFORE && input[0] && is_ifs(input[0]))
	{
		value = ft_strjoin(" ", input);
		len++;
	}
	else
		value = ft_strdup(input);
	i = 0;
	ret = NULL;
	while (input[i])
	{
		while (input[i] && is_ifs(input[i]))
			i++;
		while (input[i] && !is_ifs(input[i]))
			value[len++] = input[i++];
		while (input[i] && is_ifs(input[i]))
			i++;	
		if (input[i])
			value[len++] = ' ';
		else
		{
			value[len] = '\0';
			if (edges == AFTER || edges == BEFORE + AFTER)
				ret = ft_strjoin(value, " ");
			else
				ret = ft_strdup(value);
			value = free_and_null(&value);
		}
	}
	return (ret);
}

/*
** returns the value of a var (input) in env
** also returns the value of $?
** input[0] == '$'
*/

char	*get_var_value(char *name, char *env[], int quote, int edges)
{
	size_t	i;

	(void)quote;
	(void)edges;
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
	return (ft_strdup(""));// cas ou la variable n'existe pas vérifier s'il ne renvoie pas plutôt une chaine null
}

int		var_len_not_exp(char *input)
{
	int		len;

	len = 1;
	if (input[len] && input[len] == '?')
		return (2);
	while (input[len] && ft_isalnum(input[len]))//vérifier le cas du underscore
		len++;
	return (len);
}

/*
** gets the len of a var (input) after =
** input[0] == '$'
** if (len == 1) is for "$" with nothing afterward
*/

int		var_len_exp(char *input, char *env[], int quote, int edges)
{
	int		len;
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	len = var_len_not_exp(input);
	if (input[len] && input[len] == DOLLAR)
		quote = 1;
	//ft_printf("\nVARLEN     len = %d", len);//debug
	if (len == 1 && (!input[1] || is_ifs(input[1])))
		return (1);
	else if (len == 1)
		return (0);
	var_name = (char*)malloc(sizeof(char) * (len + 1));
	ft_strncpy(var_name, input, len);
	var_name[len] = '\0';
	//ft_printf("\nVARLEN  varname = %s", var_name);//debug
	var_value = get_var_value(var_name, env, quote, edges);
	//ft_printf("\nVARLEN varvalue = %s", var_value);//debug
	free(var_name);
	var_name = NULL;
	if (!var_value)
		return (0);
	len = ft_strlen(var_value);
	//ft_printf("\nVARLEN     len = %d", len);//debug
	free(var_value);
	var_value = NULL;
	return (len);
}

char	*get_var_name(char *input)
{
	int		i;
	int		len;
	char	*name;
	
	name = NULL;
	//ft_printf("\nGET_VAR_NAME\ninput is: %s / name is: %s", input, name);//debug
	if ((len = var_len_not_exp(input)) == 0)
		return (ft_strdup(""));
	//ft_printf("\nGET_VAR_NAME\nlen is: %d", len);//debug
	if (len == 2 && input[1] == '?')
		return (ft_strdup("$?"));
	name = (char *)malloc(sizeof(char) * (len + 1));
	name[0] = DOLLAR;
	name[len] = '\0';
	i = 1;
	while (input[i] && ft_isalnum(input[i]))
	{
		name[i] = input[i];
		i++;
	}
	//if (name && name[0] && name[0] == DOLLAR)
	//ft_printf("\ninput is: %s / name is: %s", input, name);//debug
	return (name);
}
