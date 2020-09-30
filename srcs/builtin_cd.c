/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_cd.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/28 15:08:57 by user42            #+#    #+#             */
/*   Updated: 2020/09/29 20:21:42 by user42           ###   ########.fr       */
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

void	ft_cd_error_msg(char *arg)
{
	ft_putstr_fd("minishell: cd: ", 2);
	ft_putstr_fd(arg, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(strerror(errno), 2);
	ft_putchar_fd('\n', 2);
}

int		ft_cd(t_cmd *cmd, char **env[])
{
	char	*path;
	int		ret;

	path = NULL;
	ret = 0;
	if (!cmd->argv[1] || cmd->argc > 2)
		return (ft_cd_arg_issue(cmd, env));
	path = get_expanded_token(cmd->argv[1], *env);
	if (!path || (path && !path[0]))
	{
		if (path)
			free(path);
		path = NULL;
		return (ft_cd_no_arg(env));
	}
	ret = chdir(path);
	if (ret && path)
		ft_cd_error_msg(path);
	if (path)
		free(path);
	path = NULL;
	return (ret);
}
