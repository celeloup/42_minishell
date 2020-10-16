/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   redirection.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/16 17:35:42 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/16 17:37:54 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		open_fd(int *fd, char *file, int flag, int *flux)
{
	if (flag == O_RDONLY)
		*fd = open(file, flag);
	else
		*fd = open(file, flag, 0644);
	if (*fd == -1)
	{
		error_msg(file, strerror(errno));
		return (-1);
	}
	if (flag == O_RDONLY)
		*flux = STDIN_FILENO;
	else
		*flux = STDOUT_FILENO;
	return (0);
}

int		redirect(int fd, int flux)
{
	if (dup2(fd, flux) == -1)
		return (-1);
	if (close(fd) == -1)
		return (-1);
	return (0);
}

/*
** direction 1 = in, direction 2 = out, direction 3 = both
*/

int		redirection(t_rdir *rd, int direction, int fd, int flux)
{
	if (fd != -1 && flux != -1)
		if (redirect(fd, flux) == -1)
			return (-1);
	while (rd)
	{
		if (rd->type == RDIR_IN && (direction == 1 || direction == 3))
		{
			if (open_fd(&fd, rd->value, O_RDONLY, &flux) == -1)
				return (-1);
		}
		else if (rd->type == RDIR_OUT && (direction == 2 || direction == 3))
		{
			if (open_fd(&fd, rd->value, O_WRONLY | O_CREAT | O_TRUNC,
			&flux) == -1)
				return (-1);
		}
		else if (direction == 1 || direction == 3)
			if (open_fd(&fd, rd->value, O_WRONLY | O_CREAT | O_APPEND,
			&flux) == -1)
				return (-1);
		if (redirect(fd, flux) == -1)
			return (-1);
		rd = rd->next;
	}
	return (0);
}
