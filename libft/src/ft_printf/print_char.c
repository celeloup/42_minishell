/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_char.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 12:44:33 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:45:12 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	print_s(t_print *var, va_list ap)
{
	char	*str;
	int		l;

	if (!(str = (char *)va_arg(ap, char *)))
		str = "(null)";
	l = ft_strlen(str);
	if (var->p < l && var->p >= 0)
		l = var->p;
	if (var->width > l)
	{
		var->fz = (var->fz == '0' && var->p < 0 ? '0' : ' ');
		if (var->fm == '-')
			var->len += p_r(str, var->width, ' ', l);
		else
			var->len += p_l(str, var->width, var->fz, l);
	}
	else
		var->len += p_l(str, 0, ' ', l);
}

void	print_c(t_print *var, va_list ap)
{
	char c;

	c = (char)va_arg(ap, int);
	if (c == 0)
		c = '\x00';
	if (var->width > 0)
	{
		if (var->fm == '-')
			var->len += p_r((char[2]){(char)c, '\0'},
				var->width, ' ', 1);
		else
		{
			if (var->fz == '0')
				var->len += p_l((char[2]){(char)c, '\0'},
					var->width, '0', 1);
			else
				var->len += p_l((char[2]){(char)c, '\0'},
					var->width, ' ', 1);
		}
	}
	else
		var->len += ft_putchar(c);
}

void	print_pourcent(t_print *var)
{
	if (var->width)
	{
		if (var->fm == '-')
			var->len += p_r("%", var->width, ' ', 1);
		else
		{
			if (var->fz == '0')
				var->len += p_l("%", var->width, '0', 1);
			else
				var->len += p_l("%", var->width, ' ', 1);
		}
	}
	else
		var->len += ft_putchar('%');
}
