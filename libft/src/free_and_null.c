/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free_and_null.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 16:30:59 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/15 13:12:49 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*free_and_null_str(char **str)
{
	if (*str)
		free(*str);
	return (NULL);
}

char	**free_and_null_tab(char **tab[])
{
	int	i;

	i = 0;
	if (*tab)
	{
		while ((*tab)[i])
		{
			(*tab)[i] = free_and_null_str(&((*tab)[i]));
			i++;
		}
		free(*tab);
	}
	return (NULL);
}
