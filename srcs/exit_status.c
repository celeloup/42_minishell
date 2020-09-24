/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit_status.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/24 17:37:51 by amenadier         #+#    #+#             */
/*   Updated: 2020/09/24 18:02:45 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	edit_exit_status(char **env[], int status)
{
	char	*status_str;
	int		status_len;
	int		i;

	status_str = ft_itoa(status);
	status_len = ft_strlen(status_str);
	i = 0;
	while ((*env)[i])
	{
		if (!strncmp((*env)[i], "?=", 2))
		{
			free((*env)[i]);
			(*env)[i] = ft_strjoin("?=", status_str);
			free(status_str);
			status_str = NULL;
			return ;
		}
		i++;
	}
	return ;
}
