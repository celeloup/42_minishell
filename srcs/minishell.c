/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2020/10/15 22:51:17 by celeloup         ###   ########.fr       */
=======
/*   Updated: 2020/10/15 20:02:53 by celeloup         ###   ########.fr       */
>>>>>>> 2127782fcfb861c7060373edee673f1e1a5977a1
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

//valgrind --leak-check=full --show-leak-kinds=all ./minishell > valgrind_log 2>&1

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

int		main(int argc, char *argv[], char *env[])
{
	char	*input;
	t_cmd	*cmd_list;
	char	**environment;
	int		status;

	(void)argc;
	(void)argv;
	input = NULL;
	environment = init_env(env);
	cmd_list = NULL;
	status = 0;
	signal(SIGINT, &control_c);
	signal(SIGQUIT, &control_slash);
	while (status != -1 && status != 255)
	{
		init_var();
		prompt();
		get_next_line(0, &input);
		if (!input)
			status = -1;
		else
		{
			if (g_var.sigint || g_var.sigquit)
			{
				status = g_var.status;
				edit_exit_status(&environment, g_var.status);
			}
			edit_exit_status(&environment, status);
			if (!give_cmd_birth(&cmd_list, input, &environment))
				status = exec_cmds(cmd_list, &environment);
			else
				status = get_status(&environment);
			cmd_list = free_and_null_cmd(&cmd_list);
			input = free_and_null_str(&input);
		}
	}
	input = free_and_null_str(&input);
	status = get_status(&environment);
	environment = free_and_null_tab(&environment);
	return (status);
}
