/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/07/22 16:04:29 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

/*
** Affiche le prompt en couleurs
** Si y'a eu erreur avant, affiche flèche en rouge, sinon vert
*/

void	prompt(int error)
{
	if (error == 1)
		ft_printf("%s➜  %sminishell %s> %s", RED, BLUE, YELLOW, END);
	else
		ft_printf("%s➜  %sminishell %s> %s", GREEN, BLUE, YELLOW, END);
}

int		main(int argc, char *argv[], char *env[])
{
	char	*input;
	t_cmd	*cmd_list;
	char	**environment;
	
	(void)argc;
	(void)argv;
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);
	signal(SIGTERM, signal_handler);
	/*if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
	{
		ft_putstr_fd("SIGCHLD\n", 2);
		error_exit("signal", "failed.");
	}*/
	environnement = init_env(env);
	//ft_printf("env0 = %s\n", environnement[0]);
	cmd_list = NULL;
	while (42)
	{
		prompt(0);
		get_next_line(0, &input);
		if (!input)
			control_d();
		else
		{
			//ft_printf("input is : >%s<\n", input);
			cmd_list = parse_input(input, environment);
			//print_cmd(cmd_list, 0);
			int test = 0;
			if (cmd_list)
			{
				test = exec_cmds(cmd_list, &environnement);
				cmd_list = free_cmd(cmd_list);
			}
			//ft_printf("ret test = %d\n", test);
			free(input);
			input = NULL;
		}
		//ft_printf("env0 = %s\n", environnement[0]);
		//free cmd ici ? (ft free_cmd existe dejà)
	}
	return (0);
}
