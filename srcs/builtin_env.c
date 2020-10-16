/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:44:54 by user42            #+#    #+#             */
/*   Updated: 2020/10/16 14:32:51 by celeloup         ###   ########.fr       */
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
	if (in_quote)
		ft_putchar_fd('\"', 1);
	ft_putchar_fd('\n', 1);
}

void	swap_str(char **str1, char **str2)
{
	char *tmp;

	tmp = ft_strdup(*str1);
	free(*str1);
	*str1 = ft_strdup(*str2);
	free(*str2);
	*str2 = tmp;
}

void	sort_env(char *env[])
{
	char	**tmp_env;
	int		i;
	int		j;
	int		len;

	len = 0;
	while (env[len])
		len++;
	tmp_env = env_dup(env);
	i = 0;
	while (i < len)
	{
		j = 0;
		while (j < len)
		{
			if (ft_strcmp(tmp_env[i], tmp_env[j]) < 0)
				swap_str(&tmp_env[i], &tmp_env[j]);
			j++;
		}
		i++;
	}
	print_env(tmp_env, EXP);
	tmp_env = free_and_null_tab(&tmp_env);
}

int		print_env(char *env[], int option)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "?=", 2))
		{
			if (option == EXP && ft_strncmp(env[i], "_=", 2))
				print_env_export(env[i]);
			else if (!option && ft_strchr(env[i], '='))
				ft_printf("%s\n", env[i]);
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
