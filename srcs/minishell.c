/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/15 18:29:04 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

//valgrind --leak-check=full --show-leak-kinds=all ./minishell > valgrind_log 2>&1


/*
** Affiche le prompt en couleurs
** Si y'a eu erreur avant, affiche flèche en rouge, sinon vert
*/

void	prompt(int error)
{
	/*
	if (error == 1)
		ft_printf("%s➜  %sminishell %s> %s", RED, BLUE, YELLOW, END);
	else
		ft_printf("%s➜  %sminishell %s> %s", GREEN, BLUE, YELLOW, END);
	*/
	(void)error;
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
	signal(SIGINT, control_c);
	input = NULL;
	/*signal(SIGQUIT, control_slash);
	signal(SIGTERM, signal_handler);
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
	{
		ft_putstr_fd("SIGCHLD\n", 2);
		error_exit("signal", "failed.");
	}*/
	environment = init_env(env);
	cmd_list = NULL;
	status = 0;
//	input = ft_strdup("echo $PATH");
	while (status != -1 && status != 255)//input à enlever
	{
		prompt(0);
	//	ft_printf("\ninput avant GNL is =");//debug
	//	print_args(argc, argv);//debug
		get_next_line(0, &input);
		ft_printf("\ninput is :>%s<", input);
		if (!input && !(environment = free_and_null_tab(&environment)))
			control_d();
		else
		{
			edit_exit_status(&environment, status);
	//		ft_printf("\nstatus is :%d, input is :%s", status, input);
			if (!give_cmd_birth(&cmd_list, input, &environment))
				status = exec_cmds(cmd_list, &environment);
			else
			{
				input = free_and_null_str(&input);
				input = get_var_value("$?", environment);
				status = ft_atoi(input);
			}
				
//			else if (!cmd_list)//cas de unexpected token...
//				status = 2;
			cmd_list = free_and_null_cmd(&cmd_list);
		}
		cmd_list = free_and_null_cmd(&cmd_list);
		input = free_and_null_str(&input);
	}
	input = free_and_null_str(&input);
	input = get_var_value("$?", environment);
	status = ft_atoi(input);
	input = free_and_null_str(&input);
	environment = free_and_null_tab(&environment);
	return (status);
}
