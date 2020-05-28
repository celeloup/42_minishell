/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/24 14:56:02 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:44:42 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	hub_type(t_print *var, va_list ap)
{
	if (var->type == 's')
		print_s(var, ap);
	else if (var->type == 'c')
		print_c(var, ap);
	else if (var->type == 'i' || var->type == 'd')
		print_d(var, ap);
	else if (var->type == 'u')
		print_u(var, ap);
	else if (var->type == 'x' || var->type == 'X')
		print_x(var, ap);
	else if (var->type == 'p')
		print_p(var, ap);
	else if (var->type == '%')
		print_pourcent(var);
}

void	format_iter(const char *format, t_print *var, va_list ap)
{
	while (format[var->index])
	{
		if (format[var->index] == '%')
		{
			var->index++;
			set_flag(format, var);
			set_width(format, var, ap);
			set_precision(format, var, ap);
			set_type(format, var);
			if (var->type == '0')
				return ;
			hub_type(var, ap);
			set_struct(var);
		}
		else
		{
			var->len += ft_putchar(format[var->index]);
		}
		var->index++;
	}
}

int		ft_printf(const char *format, ...)
{
	va_list ap;
	t_print var;

	va_start(ap, format);
	var.len = 0;
	var.index = 0;
	set_struct(&var);
	format_iter(format, &var, ap);
	va_end(ap);
	return (var.len);
}
