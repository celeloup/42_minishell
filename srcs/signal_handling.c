/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signal_handling.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/03 17:32:21 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 14:05:40 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	control_slash(int num)
{
	(void)num;
	if (g_var.pid != 0)
	{
		ft_putstr_fd("Quit (core dumped)\n", 2);
		g_var.status = 131;
		g_var.sigquit = 1;
	}
	else
		ft_putstr_fd("\b\b  \b\b", 2);
}

void	control_c(int num)
{
	(void)num;
	ft_putchar_fd('\n', 2);
	if (g_var.pid == 0)
	{
		prompt();
	}
	g_var.status = 130;
	g_var.sigint = 1;
}
