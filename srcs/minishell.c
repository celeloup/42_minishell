/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 12:22:22 by celeloup         ###   ########.fr       */
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

/*
** Signal interruption forte
** Pour l'instant fait rien
*/
void	control_slash(int num)
{
	ft_printf("ctrl-/\n");
	(void)num;
}

/*
** Signal arrêt
** Pour l'instant, quitte le terminal
** faut rajouter le check de si process en cours et les arrêter
*/
void	control_d()
{
	exit(EXIT_SUCCESS);
}


/*
** Signal interruption
** Pour l'instant, fait rien
** faut rajouter le check de si y'a des process en cours et les interrompre
*/
void	control_c(int num)
{
	(void)num;
	write(1, "\n", 1);
	prompt(1);
}

int		main(int argc, char *argv[], char *env[])
{
	char	*input;

	// Pour l'instant je m'en sers pas donc je les ai mute pour les flags
	(void)env;
	(void)argv;
	(void)argc;

	//INTERCEPTION DES SIGNAUX
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);


	while (42)
	{
		prompt(0);
		get_next_line(0, &input);
		if (!input) // SI ON RECOIT LE EOF = CTRL-D
			control_d();
		
		//ici mettre fonction parsing

		else if (ft_strcmp("exit", input) == 0) //CHECK SI COMMANDE = "exit"
			exit(EXIT_SUCCESS);
		ft_printf("%s\n", input);
	}
	return (0);
}