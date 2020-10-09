/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_env.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/25 12:44:54 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 20:49:21 by user42           ###   ########.fr       */
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

int		print_env(char *env[], int option)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (ft_strncmp(env[i], "?=", 2) && option == EXP)
			print_env_export(env[i]);
		else if (ft_strncmp(env[i], "?=", 2))
			ft_putstr_fd(env[i], 1);
		if (ft_strncmp(env[i], "?=", 2))
			ft_putchar_fd('\n', 1);
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
