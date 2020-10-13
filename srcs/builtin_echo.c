/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_echo.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/13 16:15:45 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_echo_first_arg(t_cmd *cmd, int *n_option)
{
	int		i;
	int		j;

	i = 1;
	if (cmd && cmd->argv[1] && !(ft_strcmp("-n", cmd->argv[1])))
		*n_option = i++;
	while (cmd && cmd->argv[i] && cmd->argv[i][0] && cmd->argv[i][0] == '-'
		&& cmd->argv[i][1] && cmd->argv[i][1] == 'n')
	{
		j = 2;
		while (cmd->argv[i][j] && cmd->argv[i][j] == 'n')
			j++;
		if (cmd->argv[i][j])
			return (i);
		i++;
	}
	return (i);
}

int		ft_echo(t_cmd *cmd, char **env[])
{
	int		i;
	int		n_option;

	(void)env;
	n_option = 0;
	i = ft_echo_first_arg(cmd, &n_option);
	while (cmd && cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i] && cmd->argv[i + 1])
			ft_putchar_fd(SPACE, 1);
		i++;
	}
	if (!n_option && ft_putchar_fd('\n', 1) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}
