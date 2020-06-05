/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/05 17:41:39 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	ft_exit(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("Exit fonction\n");
	exit(EXIT_SUCCESS);
}

void	ft_echo(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("Echo fonction\n");
}

void	ft_cd(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("cd fonction\n");
}

void	ft_pwd(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("pwd fonction\n");
}

void	ft_export(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("export fonction\n");
}

void	ft_unset(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("unset fonction\n");
}

void	ft_env(t_cmd *cmd, char *env[])
{
	(void)cmd;
	(void)env;
	ft_printf("env fonction\n");
}
