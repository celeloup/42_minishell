/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 17:46:24 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

/*
** valgrind --leak-check=full --show-leak-kinds=all ./minishell
** > valgrind_log 2>&1
*/

t_var	g_var;

void	init_var(void)
{
	g_var.pid = 0;
	g_var.status = 0;
	g_var.sigint = 0;
	g_var.sigquit = 0;
}

void	prompt(void)
{
	ft_putstr_fd("➜ minishell > ", 2);
}

int		launch_mini(char **env[], char *input)
{
	int		status;
	t_cmd	*cmd_list;

	status = get_status(env);
	if (g_var.sigint)
	{
		status = g_var.status;
		edit_exit_status(env, g_var.status);
		init_var();
	}
	edit_exit_status(env, status);
	if (give_cmd_birth(&cmd_list, input, env))
		status = get_status(env);
	else
	{
		input = free_and_null_str(&input);
		status = exec_cmds(cmd_list, env);
		if (status != 255)
			edit_exit_status(env, status);
	}
	cmd_list = free_and_null_cmd(&cmd_list);
	return (status);
}

int		minishell(char **env[])
{
	int		status;
	char	*input;

	status = get_status(env);
	init_var();
	prompt();
	get_next_line(0, &input);
	if (!input)
	{
		ft_putstr_fd("exit \n", 2);
		status = -1;
	}
	else
		status = launch_mini(env, input);
	return (status);
}

int		main(int argc, char *argv[], char *env[])
{
	char	**environment;
	int		status;

	(void)argc;
	(void)argv;
	environment = init_env(env);
	status = 0;
	signal(SIGINT, &control_c);
	signal(SIGQUIT, &control_slash);
	while (status != -1 && status != 255)
		status = minishell(&environment);
	status = get_status(&environment);
	environment = free_and_null_tab(&environment);
	return (status);
}
