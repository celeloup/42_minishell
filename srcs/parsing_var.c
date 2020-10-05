/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_var.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 14:33:25 by user42            #+#    #+#             */
/*   Updated: 2020/10/05 20:29:44 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*word_splitted_var_value(char *input)
{
	char	*value;
	char	*ret;
	int		i;
	int		len;

	value = ft_strdup(input);
	i = 0;
	len = 0;
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
			ret = ft_strdup(value);
			free(value);
		}
	}
	return (ret);
}

/*
** returns the value of a var (input) in env
** also returns the value of $?
** input[0] == '$'
*/

char	*get_var_value(char *input, int quote, char *env[])
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
			{
				if (quote)
					return (ft_strdup(ft_strchr(env[i], '=') + 1));
				return (word_splitted_var_value(ft_strchr(env[i], '=') + 1));
			}
		i++;
	}
	
	return (NULL);//vérifier s'il ne renvoie pas plutôt une chaine vide
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
** if (expanded && len == 1) is for "$" with nothing afterward
*/

int		var_len(char *input, char *env[], int quote, int expanded)
{
	int		len;
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	len = var_len_not_exp(input);
	//ft_printf("\nVARLEN     len = %d", len);//debug
	if (!expanded)
		return (len);
	if (expanded && len == 1)
		return (1);
	var_name = (char*)malloc(sizeof(char) * (len + 1));
	ft_strncpy(var_name, input, len);
	var_name[len] = '\0';
	//ft_printf("\nVARLEN  varname = %s", var_name);//debug
	var_value = get_var_value(var_name, quote, env);
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
	len = var_len(input, NULL, NO, NOT_EXP);
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
