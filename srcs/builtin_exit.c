/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_exit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/17 12:33:11 by celeloup         ###   ########.fr       */
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

int		ft_str_is_nb(char *str)
{
	int i;

	i = 0;
	if (str[i] == '-' || str[i] == '+')
		i++;
	while (str[i])
	{
		if (!ft_isdigit(str[i]))
		{
			return (0);
			break ;
		}
		i++;
	}
	return (1);
}

int		ft_exit(t_cmd *cmd, char **env[])
{
	int	status;

	status = 0;
	if (cmd && cmd->argv && cmd->argv[1])
	{
		if (!ft_str_is_nb(cmd->argv[1]))
		{
			error_msg("exit", ": numeric argument required");
			status = 2;
		}
		else if (cmd->argv[2])
		{
			error_msg("exit", "too many arguments");
			status = 1;
		}
		else
			status = ft_atoi(cmd->argv[1]);
		if (status == -1)
			error_msg("exit", ": numeric argument required");
		edit_exit_status(env, status);
	}
	return (-1);
}
