/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_adult_argv.c                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/15 19:52:56 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		adult_len(char *teen, char *env[])
{
	int		i;
	int		len;
	char	*part;

	i = 0;
	len = 0;
	while (teen[i] && !(is_ifs(teen[i])))
	{
		part = get_adult_part(&teen[i], env, NO);
		if (part)
			len += ft_strlen(part);
		part = free_and_null_str(&part);
		i += go_to_next_char(&teen[i], NO);
	}
	return (len);
}

char	*get_adult_part(char *teen, char *env[], int quote)
{
	char	*part;
	char	*var_name;

	var_name = NULL;
	if (!teen || !teen[0])
		return (0);
	else if (teen[0] == BACKSLASH)
		part = get_escaped_char(teen, quote);
	else if (is_quote(teen[0]) && !quote)
		part = get_quote(teen, env);
	else if (teen[0] == DOLLAR && quote)
	{
		var_name = get_var_name(teen);
		part = get_var_value(var_name, env);
		var_name = free_and_null_str(&var_name);
	}
	else
		part = ft_substr(teen, 0, go_to_next_char(teen, quote));
	ft_printf("\npart is : %s", part);
	return (part);
}

char	*get_one_adult_arg(char *teen, char *env[])
{
	char	*adult;
	char	*part;
	int		len;
	int		i;

	adult = NULL;
	i = 0;
	ft_printf("\nget one adult teen is >%s<", teen);
	while (teen && teen[i] && is_ifs(teen[i]))
		i++;
	if (!teen || !teen[i])
		return (NULL);
	len = adult_len(&teen[i], env);
	adult = (char*)malloc(sizeof(char) * (len + 1));
	adult[len] = '\0';
	ft_printf("\nget one adult len is >%d<", len);
	while (len)
		adult[--len] = ' ';
	len = 0;
	ft_printf("\nget one adult end adult is >%s<", adult);
	while (teen[i] && adult[len])
	{
		if ((part = get_adult_part(&teen[i], env, NO)))
		{
			ft_strncpy(&adult[len], part, ft_strlen(part));
			len += ft_strlen(part);
		}
		part = free_and_null_str(&part);
		ft_printf("\nget one adult end adult is >%s<", adult);
		i += go_to_next_char(&teen[i], NO);
	}
	ft_printf("\nget one adult end adult is >%s<", adult);
	return (adult);
}

char	**split_teen(char **adult, char *teen, char *env[], int j)
{
	int		i;

	i = 0;
	ft_printf("\nsplit teen teen is :%s", teen);
	while (teen[i] && is_ifs(teen[i]))
		i++;
	if (!teen[i])
		adult[j] = ft_strdup("");
	while (teen[i])
	{
		if (!(adult[j] = get_one_adult_arg(&teen[i], env)))
			break ;
		ft_printf("\nsplit teen adult[j] is :>%s<", adult[j]);
		j++;
		i += child_len(&teen[i]);
		while (teen[i] && is_ifs(teen[i]))
			i++;
	}
	ft_printf("\nv--split teen adult");
	print_args(2, adult);
	return (adult);
}

char	**get_adult_argv(char **baby, int adult_argc, char *env[])
{
	char	**adult;
	char	*teen;
	int		i;
	int		j;

	i = 0;
	j = 0;
	teen = NULL;
	adult = init_argv(adult_argc);
	while (baby[i] && j < adult_argc)
	{
		while (baby[i] && !(teen = make_baby_a_teen(baby[i], env)))
			i++;
		if (!baby[i])
			break ;
		ft_printf("\nGet adult teen is :%s", teen);
		split_teen(adult, teen, env, j);
		teen = free_and_null_str(&teen);
		j += words_in_baby(baby[i], env);
		i++;
	}
	ft_printf("\nGetadult args end :");
	print_args(adult_argc, adult);
	return (adult);
}
