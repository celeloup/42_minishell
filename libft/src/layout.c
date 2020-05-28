/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   layout.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/31 14:18:35 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:34:07 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

/*
** p_r writes the string aligned to the right
*/

int		p_r(char *str, int len, char c, int str_limit)
{
	int i;

	i = 0;
	while (i < str_limit)
	{
		ft_putchar(str[i]);
		i++;
	}
	while (i < len)
	{
		ft_putchar(c);
		i++;
	}
	return (i);
}

/*
** p_l writes the string aligned to the left (0 or ' ')
** (and considers the '-' in a negative string)
*/

int		p_l(char *str, int len, char c, int str_limit)
{
	int i;
	int j;

	j = str_limit;
	i = 0;
	if (c == '0' && str[0] == '-')
	{
		i += ft_putchar('-');
		j--;
	}
	while (i < len - j)
	{
		ft_putchar(c);
		i++;
	}
	if (str[0] == '-' && c == '0')
		j = 1;
	else
		j = 0;
	while (j < str_limit)
	{
		i += ft_putchar(str[j]);
		j++;
	}
	return (i);
}

void	fill_zero(char *str, int zero)
{
	int i;

	i = 0;
	while (i < zero)
	{
		str[i] = '0';
		i++;
	}
}

/*
** Append_zero adds zeros at the begining of a string
** Takes a pointer to a string.
** Copy the content of the string in a new string called copy,
** free the original string (no leaks) then malloc a new string
** where the original string was, add zeros at the begining (considers
** if the string is a negative number) and copy the original string back.
** It returns the number of zeros it added to the string.
*/

int		append_zero(char **str, int zero)
{
	char	*copy;
	int		len;

	len = ft_strlen(*str);
	if ((copy = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
		return (-1);
	ft_strcpy(copy, *str);
	free(*str);
	if (copy[0] == '-')
		zero++;
	if (!(*str = (char*)malloc(sizeof(char) * (len + zero + 1))))
		return (-1);
	if (copy[0] != '-')
	{
		fill_zero(*str, zero);
		ft_strcpy(*str + zero, copy);
	}
	else
	{
		*str[0] = '-';
		fill_zero(*str + 1, zero);
		ft_strcpy(*str + zero + 1, copy + 1);
	}
	free(copy);
	return (zero);
}
