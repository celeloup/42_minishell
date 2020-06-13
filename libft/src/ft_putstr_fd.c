/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_putstr_fd.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2019/12/02 17:39:09 by celeloup          #+#    #+#             */
/*   Updated: 2019/12/02 17:39:13 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

int		ft_putstr_fd(char const *s, int fd)
{
	int i;
	int	ret;

	i = 0;
	ret = 0;
	if (!s)
		return (0);
	else
	{
		while (s[i])
		{
			if (ft_putchar_fd(s[i], fd) < 0)
				ret = -1;
			i++;
		}
	}
	return (ret);
}
