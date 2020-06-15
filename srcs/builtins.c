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

void	ft_exit(t_cmd *cmd, char **env[])
{
	
	(void)env;
	if (kill(0, SIGTERM))
	{
		env_error(NULL, cmd->argv[0], errno);
		exit(EXIT_FAILURE);
	}
	if (cmd && cmd->argv && cmd->argv[1])
		exit(atoi(cmd->argv[1]));
	else
		exit(EXIT_SUCCESS);
}

int		ft_echo(t_cmd *cmd, char **env[])
{
	int		ret;
	int		i;
	int		n_option;

	ret = 0;
	i = 1;
	n_option = 0;
	if (cmd && cmd->argv[1] && !(ft_strcmp("-n", cmd->argv[1])))
		n_option = i++;
	while (cmd && cmd->argv[i])
	{
		if (ft_putstr_fd(cmd->argv[i], 1) < 0)
			ret = 1;
		if (cmd->argv[i + 1] && ft_putchar_fd(SPACE, 1) < 0)
			ret = 1;
		i++;
	}
	if (!n_option && ft_putchar_fd('\n', 1) < 0)
		ret = 1;
	(void)env;
	exit(ret);
}

void	ft_cd(t_cmd *cmd, char **env[])
{
	int		ret;
	char	*old_path = NULL;

	ret = 0;
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
	exit(ret);
	(void)env;
}

/*
**
*/

	void
ft_pwd(t_cmd *cmd, char **env[])
{
	if (getcwd(NULL, 0))// verifier que pas de leak sinon faire un char *tmp
	{
		ft_putstr_fd(getcwd(NULL, 0), 1);
		ft_putchar_fd('\n', 1);
	}
	else
	{
		env_error(NULL, cmd->argv[0], errno);
		exit(EXIT_FAILURE);
	}
	exit(EXIT_SUCCESS);
	(void)env;
}

	int	
ft_export(t_cmd *cmd, char **env[])
{
	int		i;
	int		ret;

	i = 1;
	ret = 0;
	if (!cmd->argv)
		exit(1);
	if (!cmd->argv[1])
		print_env(*env, EXP);
	while (cmd->argv[i])
	{
		if (add_var(env, cmd->argv[0], cmd->argv[i]) > 0)
			ret = 1;
		i++;
	}
	print_env(*env, 0);
	exit(ret);
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
	ret = 0;
	if (!cmd->argv)
		exit(EXIT_FAILURE);
	if (!cmd->argv[1])
		exit(EXIT_SUCCESS);
	while (cmd->argv[i])
	{
		if (remove_var(env, cmd->argv[0], cmd->argv[i], NO) > 0)
			ret = 1;
		i++;
	}
	//print_env(*env, 0);
	exit(ret);
}

void	ft_env(char **env[])
{
	print_env(*env, NO);
	exit(EXIT_SUCCESS);
}