/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_pwd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/12 21:00:39 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	edit_pwd_and_oldpwd(char *curpath, char *cmd, char **env[])
{
	char	*pwd;
	char	*old;

	pwd = get_var_value("$PWD", *env);
	old = ft_strjoin("OLDPWD=", pwd);
	edit_var(env, cmd, old);
	old = free_and_null_str(&old);
	pwd = free_and_null_str(&pwd);
	pwd = ft_strjoin("PWD=", curpath);
	edit_var(env, cmd, pwd);
	pwd = free_and_null_str(&pwd);
}

/*
** getcwd returns a string if success, else NULL
*/

int		ft_pwd(t_cmd *cmd, char **env[])
{
	char *tmp;

	tmp = NULL;
	if (!(tmp = getcwd(NULL, 0)))
		tmp = get_var_value("$PWD", *env);
	if (tmp)
	{
		ft_putstr_fd(tmp, 1);
		ft_putchar_fd('\n', 1);
		tmp = free_and_null_str(&tmp);
	}
	else
	{
		print_env_error(NULL, cmd->argv[0], errno);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
