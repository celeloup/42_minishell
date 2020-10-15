/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 17:32:21 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/15 13:33:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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
	prompt(0);
}

void	signal_handler(int num)
{
	if (num == SIGTERM)
	{
		//free here
		//ft_printf("sigterm signal received \n");
		exit(EXIT_SUCCESS);
	}
}
