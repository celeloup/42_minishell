/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtins.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/05 17:41:04 by celeloup          #+#    #+#             */
/*   Updated: 2020/08/06 11:19:37 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int	 ft_exit(t_cmd *cmd, char **env[])
{
	(void)cmd;
	(void)env;
	//free_cmd(cmd);
	//free_env(*env);
	/*
	if (cmd && cmd->argv && cmd->argv[1])
		exit(atoi(cmd->argv[1]));
	*/
	/* ???? don't know what it does and have a weird behavior 
	if (kill(0, SIGTERM))
	{
		print_env_error(NULL, cmd->argv[0], errno);
		exit(EXIT_SUCCESS);
	}*/
	//else
		//exit(EXIT_SUCCESS);
	return (-1);
}

int		ft_echo(t_cmd *cmd, char **env[])
{
	int		ret;
	int		i;
	int		n_option;
	(void)env;
	ret = EXIT_SUCCESS;
	i = 1;
	n_option = 0;
	if (cmd && cmd->argv[1] && !(ft_strcmp("-n", cmd->argv[1])))
		n_option = i++;
	while (cmd && cmd->argv[i])
	{
		if (ft_putstr_fd(cmd->argv[i], 1) < 0)
			ret = EXIT_FAILURE;
		if (cmd->argv[i + 1] && ft_putchar_fd(SPACE, 1) < 0)
			ret = EXIT_FAILURE;
		i++;
	}
	if (!n_option && ft_putchar_fd('\n', 1) < 0)
		ret = EXIT_FAILURE;
	//exit(ret);
	return (ret);
}

/*
** CD, Cd and cD won't do anything if the directory exists (No error msg) but 
** they won't change the working directory accordingly either. Hence the last if
*/

int	ft_cd(t_cmd *cmd, char **env[])
{
	int		ret;
	char	*old_path = NULL;
	(void)env;
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
	//exit(ret);
	return (ret);
}

/*
** getcwd returns a string if success, else NULL
*/
	int
ft_pwd(t_cmd *cmd, char **env[])
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
	//exit(EXIT_SUCCESS);
	return (1);
}

	int	
ft_export(t_cmd *cmd, char **env[])
{
	int		i;
	int		ret;

	i = 1;
	ret = EXIT_SUCCESS;
	if (!cmd->argv)
		exit(EXIT_SUCCESS);
	if (!cmd->argv[1])
		print_env(*env, EXP);
	while (cmd->argv[i])
	{
		if (add_var(env, cmd->argv[0], cmd->argv[i]) > 0)
		{
			print_env_error(cmd->argv[i], cmd->argv[0], errno);
			ret = EXIT_FAILURE;
		}
		i++;
	}
	//exit(ret);
	return (ret);
}

/*
** MAN UNSET :
** Unsetting a variable or function that was not previously set shall
**       not be considered an error and does not cause the shell to abort.
** Note that:
**           VARIABLE=
**       is not equivalent to an unset of VARIABLE; in the example, VARIABLE
**       is set to "".
*/
	int
ft_unset(t_cmd *cmd, char **env[])
{
	int		i;
	int		ret;

	i = 1;
	ret = EXIT_SUCCESS;
	if (!cmd->argv)
		exit(EXIT_FAILURE);
	if (!cmd->argv[1])
		exit(EXIT_SUCCESS);
	while (cmd->argv[i])
	{
		if (remove_var(env, cmd->argv[0], cmd->argv[i], NO) > 0)
		{
			print_env_error(cmd->argv[i], cmd->argv[0], errno);
			ret = EXIT_FAILURE;
		}
		i++;
	}
	//exit(ret);
	return (ret);
}

int	ft_env(t_cmd *cmd, char **env[])
{
	(void)cmd;
	print_env(*env, NO);
	//exit(EXIT_SUCCESS);
	return (1);
}