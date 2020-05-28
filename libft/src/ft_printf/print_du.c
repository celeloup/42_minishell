/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_du.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 18:20:24 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:45:17 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

int		print_d_zero(char *str, int width, int len)
{
	if (width > len)
		return (p_r(str, width, ' ', 0));
	else
		return (p_l(str, width, ' ', 0));
}

void	print_d(t_print *var, va_list ap)
{
	int		nb;
	char	*str;
	int		l;

	nb = (int)va_arg(ap, int);
	str = ft_itoa(nb);
	l = ft_strlen(str);
	if (nb == 0 && var->p == 0)
		var->len += print_d_zero("", var->width, l);
	else
	{
		if (var->p >= l && var->p > 0)
			l += append_zero(&str, var->p - l);
		if (var->width > l)
		{
			var->fz = (var->fz == '0' && var->p < 0 ? '0' : ' ');
			if (var->fm == '-')
				var->len += p_r(str, var->width, ' ', l);
			else
				var->len += p_l(str, var->width, var->fz, l);
		}
		else
			var->len += ft_putstr(str);
	}
	free(str);
}

void	print_u(t_print *var, va_list ap)
{
	int		nb;
	char	*str;
	int		l;

	nb = (unsigned int)va_arg(ap, unsigned int);
	str = ft_u_itoa(nb);
	l = ft_strlen(str);
	if (var->p == 0 && nb == 0)
		var->len += p_r("", var->width, ' ', 0);
	else
	{
		if (var->p > l)
			l += append_zero(&str, var->p - l);
		if (var->width >= l)
		{
			var->fz = (var->fz == '0' && var->p < 0 ? '0' : ' ');
			if (var->fm == '-')
				var->len += p_r(str, var->width, ' ', l);
			else
				var->len += p_l(str, var->width, var->fz, l);
		}
		else
			var->len += ft_putstr(str);
	}
	free(str);
}
