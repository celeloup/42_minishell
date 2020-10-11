/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:44:54 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 12:43:56 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	print_env_export(char *var)
{
	int	in_quote;
	int	i;

	in_quote = 0;
	i = 0;
	ft_putstr_fd("declare -x ", 1);
	while (var[i])
	{
		ft_putchar_fd(var[i], 1);
		if (var[i] == '=' && !in_quote)
		{
			ft_putchar_fd('\"', 1);
			in_quote++;
		}
		i++;
	}
	ft_putchar_fd('\"', 1);
}

void	sort_env(char *env[])
{
	char **tmp_env;
	int i;
	int j;
	int len ;
	char *tmp_str;

	len = 0;
	while (env[len])
		len++;
	if ((tmp_env = malloc((len + 1) * sizeof(char*))) == NULL)
		return ;
	i = 0;
	while (i < len)
	{
		tmp_env[i] = ft_strdup(env[i]);
		i++;
	}
	tmp_env[i] = NULL;
	i = 0;
	while (i < len)
	{
		j = 0;
		while (j < len)
		{
			if (ft_strcmp(tmp_env[i], tmp_env[j]) < 0)
			{
				tmp_str = ft_strdup(tmp_env[i]);
				free(tmp_env[i]);
				tmp_env[i] = ft_strdup(tmp_env[j]);
				free(tmp_env[j]);
				tmp_env[j] = tmp_str;
			}
			j++;
		}
		i++;
	}
	print_env(tmp_env, EXP);
	free_env(tmp_env);
}

int		print_env(char *env[], int option)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "?=", 2))
		{
			if (option == EXP)
				print_env_export(env[i]);
			else
				ft_putstr_fd(env[i], 1);
			ft_putchar_fd('\n', 1);
		}
		i++;
	}
	return (EXIT_SUCCESS);
}

int		ft_env(t_cmd *cmd, char **env[])
{
	(void)cmd;
	print_env(*env, NO);
	return (EXIT_SUCCESS);
}
