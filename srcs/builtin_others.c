/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/25 17:01:23 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	edit_exit_status(char **env[], int status)
{
	char	*status_str;
	int		status_len;
	int		i;

	status_str = ft_itoa(status);
	status_len = ft_strlen(status_str);
	i = 0;
	while ((*env)[i])
	{
		if (!strncmp((*env)[i], "?=", 2))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("?=", status_str);
			free(status_str);
			status_str = NULL;
			return ;
		}
		i++;
	}
	return ;
}

int		ft_exit(t_cmd *cmd, char **env[])
{
	int		status;

	if (cmd && cmd->argv && cmd->argv[1])
	{
		status = ft_atoi(cmd->argv[1]);
		edit_exit_status(env, status);
	}
	return (-1);
}

int		ft_echo(t_cmd *cmd, char **env[])
{
	int		i;
	int		n_option;

	(void)env;
	i = 1;
	n_option = 0;
	if (cmd && cmd->argv[1] && !(ft_strcmp("-n", cmd->argv[1])))
		n_option = i++;
	while (cmd && cmd->argv[i])
	{
		if (ft_putstr_fd(cmd->argv[i], 1) < 0)
			return (EXIT_FAILURE);
		if (cmd->argv[i + 1] && ft_putchar_fd(SPACE, 1) < 0)
			return (EXIT_FAILURE);
		i++;
	}
	if (!n_option && ft_putchar_fd('\n', 1) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
}

/*
** CD, Cd and cD won't do anything if the directory exists (No error msg) but
** they won't change the working directory accordingly either. Hence the last if
*/

int		ft_cd(t_cmd *cmd, char **env[])
{
	int		ret;
	char	*old_path;

	(void)env;
	old_path = NULL;
	ret = EXIT_SUCCESS;
	if (cmd->argv[1])
		ret = chdir(cmd->argv[1]);
	if (ret)
	{
		ret = errno;
		ft_putstr_fd(strerror(errno), 2);
		ft_putchar_fd('\n', 2);
	}
	if (ret || ft_strcmp(cmd->argv[0], "cd"))
	{
		old_path = getcwd(NULL, 0);
		chdir(old_path);
		free(old_path);
		old_path = NULL;
	}
	return (ret);
}

/*
** getcwd returns a string if success, else NULL
*/
int		ft_pwd(t_cmd *cmd, char **env[])
{
	char *tmp;

	(void)env;
	if ((tmp = getcwd(NULL, 0)))
	{
		ft_putstr_fd(tmp, 1);
		ft_putchar_fd('\n', 1);
		free(tmp);
	}
	else
	{
		print_env_error(NULL, cmd->argv[0], errno);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
