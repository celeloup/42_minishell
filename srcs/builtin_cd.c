/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 15:08:57 by user42            #+#    #+#             */
/*   Updated: 2020/10/09 20:48:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		ft_cd_no_arg(char **env[])
{
	int		ret;
	char	*path;

	path = NULL;
	path = get_var_value("$HOME", *env);
	ret = 0;
	if (path && path[0])
		ret = chdir(path);
	else if (!path)
	{
		free(path);
		path = NULL;
		ft_putstr_fd("minishell: cd: HOME not set\n", 2);
		return (1);
	}
	free(path);
	path = NULL;
	return (ret);
}

int		ft_cd_arg_issue(t_cmd *cmd, char **env[])
{
	if (!cmd->argv[1])
		return (ft_cd_no_arg(env));
	else if (cmd->argc > 2)
	{
		ft_putstr_fd("minishell: cd: too many arguments\n", 2);
		return (1);
	}
	return (0);
}

int		ft_cd_error_msg(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
	return (1);
}

int		ft_cd(t_cmd *cmd, char **env[])
{
	int		ret;

	ret = 0;
	if (!cmd->argv[1] || cmd->argc > 2)
		return (ft_cd_arg_issue(cmd, env));
	if (!(ft_strcmp(cmd->argv[1], "-")))
	{
		cmd->argv[1] = free_and_null_str(&cmd->argv[1]);
		cmd->argv[1] = get_var_value("$OLDPWD", *env);
		ft_printf("%s\n", cmd->argv[1]);// ! Pas du debug ! Comportement normal de shell
	}
	ret = chdir(cmd->argv[1]);
	if (ret && cmd->argv[1])
		return (ft_cd_error_msg(cmd->argv[1]));
	return (ret);
}
