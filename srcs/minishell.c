/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/03 18:46:11 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	t_arg	arg;
	
	(void)argc;
	(void)argv;

	//Pour comparaison (marche si args minishell idem args lancement programme)
	//print_args(argc, argv, "Real ");
	
	//INTERCEPTION DES SIGNAUX
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);

	//Pour le moment parse en dehors de boucle et GNL
	//=> permet de vérifier que argc de notre minishell = argc du vrai minishell
	init_arg(&arg);
	while (42)
	{
		prompt(0);
		get_next_line(0, &input);
		if (!input) // SI ON RECOIT LE EOF = CTRL-D
			control_d();
		else if (ft_strcmp("exit", input) == 0) //CHECK SI COMMANDE = "exit"
			exit(EXIT_SUCCESS);
		else //parsing
		{
			//ft_printf("input is : >%s<\n", input);
			parse(input, &arg);
			//print_args(arg.count, arg.value, "(Our) mini");
		}
		if (fork() == 0)
		{
			if (execve(arg.value[0], arg.value, env) == -1)
			{
				char *test = strerror(errno);
				ft_printf("error : %s\n", test);
			}
			return (0);
		}
		else
		{
			wait(NULL);
		}
	}
	free_arg(&arg);
	return (0);
}
