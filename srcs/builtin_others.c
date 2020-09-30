/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_others.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/12 12:26:26 by celeloup         ###   ########.fr       */
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
			status_str = free_and_null_str(&status_str);
			return ;
		}
		i++;
	}
	return ;
}

int		ft_exit(t_cmd *cmd, char **env[])
{
	int	status;

	if (cmd && cmd->argv && cmd->argv[1])
	{
		status = ft_atoi(cmd->argv[1]);
		edit_exit_status(env, status);
	}
	return (-1);
}

int		ft_echo_first_arg(t_cmd *cmd, int *n_option)
{
	int		i;
	int		j;

	i = 1;
	if (cmd && cmd->argv[1] && !(ft_strncmp("-n", cmd->argv[1], 2)))
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
		if (cmd->argv[i][0] && cmd->argv[i + 1])
			ft_putchar_fd(SPACE, 1);//vérifier ce point
		i++;
	}
	if (!n_option && ft_putchar_fd('\n', 1) < 0)
		return (EXIT_FAILURE);
	return (EXIT_SUCCESS);
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
		tmp = free_and_null_str(&tmp);
	}
	else
	{
		print_env_error(NULL, cmd->argv[0], errno);
		exit(EXIT_FAILURE);
	}
	return (EXIT_SUCCESS);
}
