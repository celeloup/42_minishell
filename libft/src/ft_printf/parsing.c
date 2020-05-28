/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/30 15:00:50 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:45:08 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "ft_printf.h"

void	set_struct(t_print *pf_var)
{
	pf_var->fm = 'a';
	pf_var->fz = ' ';
	pf_var->width = 0;
	pf_var->p = -1;
	pf_var->type = '0';
}

void	set_width(const char *format, t_print *var, va_list ap)
{
	int		k;
	char	*str;

	if (format[var->index] && ft_isdigit(format[var->index]))
	{
		k = var->index;
		while (format[k] && ft_isdigit(format[k]))
			k++;
		str = ft_substr(format, var->index, k);
		var->width = ft_atoi(str);
		free(str);
		var->index += k - var->index;
	}
	else if (format[var->index] == '*')
	{
		var->width = (int)va_arg(ap, int);
		if (var->width < 0)
		{
			var->fm = '-';
			var->width = var->width * -1;
		}
		var->index++;
	}
}

void	set_flag(const char *format, t_print *var)
{
	while (format[var->index] && (format[var->index] == '-'
		|| format[var->index] == '0'))
	{
		if (format[var->index] && format[var->index] == '-')
		{
			var->fm = '-';
			var->index++;
		}
		if (format[var->index] && format[var->index] == '0')
		{
			var->fz = '0';
			var->index++;
		}
	}
}

void	set_precision(const char *format, t_print *var, va_list ap)
{
	int		k;
	char	*str;

	if (format[var->index] && format[var->index] == '.')
	{
		if (format[++var->index] == '*')
		{
			var->p = (int)va_arg(ap, int);
			var->index++;
			return ;
		}
		set_flag(format, var);
		k = var->index;
		while (format[k] && ft_isdigit(format[k]))
			k++;
		if (k == var->index)
			var->p = 0;
		else
		{
			str = ft_substr(format, var->index, k);
			var->p = ft_atoi(str);
			free(str);
		}
		var->index += k - var->index;
	}
}

void	set_type(const char *format, t_print *var)
{
	if (format[var->index] && (ft_isalpha(format[var->index])
		|| format[var->index] == '%'))
	{
		var->type = format[var->index];
	}
	else
		var->type = '0';
}
