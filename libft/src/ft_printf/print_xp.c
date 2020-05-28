/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   print_xp.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/02/07 18:07:31 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:45:22 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

char	*convert_x(char type, unsigned int nb)
{
	if (type == 'x')
		return (ft_itoa_base(nb, "0123456789abcdef"));
	else
		return (ft_itoa_base(nb, "0123456789ABCDEF"));
}

void	print_x(t_print *var, va_list ap)
{
	unsigned int	nb;
	char			*str;
	int				l;

	nb = (unsigned int)va_arg(ap, unsigned int);
	str = convert_x(var->type, nb);
	l = ft_strlen(str);
	if (nb == 0 && var->p == 0)
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

char	*app_p(char **str)
{
	int		len;
	char	*copy;

	len = ft_strlen(*str);
	if ((copy = (char*)malloc(sizeof(char) * (len + 1))) == NULL)
		return (NULL);
	ft_strcpy(copy, *str);
	free(*str);
	if (!(*str = (char*)malloc(sizeof(char) * (len + 2 + 1))))
		return (NULL);
	ft_strcpy(*str, "0x");
	ft_strcpy(*str + 2, copy);
	free(copy);
	return (*str);
}

void	print_p(t_print *var, va_list ap)
{
	unsigned long long	nb;
	char				*str;
	int					l;

	nb = (unsigned long long)va_arg(ap, unsigned long long);
	str = ft_itoa_base(nb, "0123456789abcdef");
	l = ft_strlen(str);
	if (nb == 0 && var->p == 0)
		var->len += p_l("0x", var->width, ' ', 2);
	else
	{
		if (var->p > l)
			l += append_zero(&str, var->p - l);
		if (var->width > l + 2)
		{
			var->fz = (var->fz == '0' && var->p < 0 ? '0' : ' ');
			if (var->fm == '-')
				var->len += p_r(app_p(&str), var->width, ' ', l + 2);
			else
				var->len += p_l(app_p(&str), var->width, var->fz, l + 2);
		}
		else
			var->len += ft_putstr(app_p(&str));
	}
	free(str);
}
