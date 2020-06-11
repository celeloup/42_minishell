/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/10 18:34:12 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_putstr_fd("Exit fonction\n", 2);
	kill(0, SIGTERM);
	exit(EXIT_SUCCESS);
}

void	ft_echo(t_cmd *cmd, char *env[])
{
	int		i;
	int		n_option;

	i = 1;
	n_option = 0;
	if (cmd->argv[1] && !(ft_strcmp("-n", cmd->argv[1])))
		n_option = i++;
	while (cmd->argv[i])
	{
		ft_putstr_fd(cmd->argv[i], 1);
		if (cmd->argv[i + 1])
			ft_putchar_fd(SPACE, 1);
		i++;
	}
	if (!n_option)
			ft_putchar_fd('\n', 1);
	(void)env;
	exit(EXIT_SUCCESS);
}

void	ft_cd(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	//ft_putstr_fd("cd fonction\n", 2);
	exit(EXIT_SUCCESS);
}

void	ft_pwd(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_putstr_fd("pwd fonction\n", 2);
	exit(EXIT_SUCCESS);
}

void	ft_export(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_putstr_fd("export fonction\n", 2);
	exit(EXIT_SUCCESS);
}

void	ft_unset(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_putstr_fd("unset fonction\n", 2);
	exit(EXIT_SUCCESS);
}

void	ft_env(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_putstr_fd("env fonction\n", 2);
	exit(EXIT_SUCCESS);
}
