/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/11 10:26:33 by celeloup         ###   ########.fr       */
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
/*
t_cmd	*construct_list(char *input)
{
	t_cmd	*cmd;
	t_cmd	*cmd2;
	t_rdir	*rd1;
	t_rdir	*rd2;

	rd1 = malloc(sizeof(t_cmd));
	rd1->type = RDIR_OUT;
	rd1->value = "test";
	rd2 = malloc(sizeof(t_cmd));
	rd1->next = NULL;
	rd2->type = RDIR_OUT;
	rd2->value = "second_test";
	rd2->next = NULL;
	cmd = malloc(sizeof(t_cmd));
	cmd2 = malloc(sizeof(t_cmd));
	char **arg = malloc(sizeof(char*) * 4);
	arg[0] = ft_strdup("/bin/echo");
	arg[1] = NULL;
	arg[2] = ft_strdup("eheh");
	arg[3] = NULL;
	cmd->argv = arg;
	//cmd->argv = ft_split(input, ' ');
	(void)input;
	cmd->pipe = 0;
	cmd->rdir = NULL;
	cmd->next = NULL;
	cmd2->argv = ft_split("/usr/bin/wc Makefile", ' ');
	cmd2->pipe = 0;
	t_cmd *cmd3 = malloc(sizeof(t_cmd));
	cmd3->argv = ft_split("/bin/echo cmd3", ' ');
	cmd3->next = NULL;
	cmd->pipe = 0;
	cmd2->next = cmd3;
	return (cmd);
}
*/
int		main(int argc, char *argv[], char *env[])
{
	char	*input;
	t_cmd	*cmd_list;
	char	**environnement;

	(void)argc;
	(void)argv;
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);
	signal(SIGTERM, signal_handler);
	if (signal(SIGCHLD, SIG_IGN) == SIG_ERR)
	{
		ft_putstr_fd("SIGCHLD\n", 2);
		error_exit("signal", "failed.");
	}
	environnement = init_env(env);
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
			cmd_list = parse_input(input, environnement);
			exec_cmds(cmd_list, environnement);
			//print_cmd(cmd_list, 0);
		}
		//free cmd ici ? (ft free_cmd existe dejà)
	}
	return (0);
}
