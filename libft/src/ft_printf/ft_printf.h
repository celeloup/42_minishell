/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/01/23 15:57:27 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 10:44:55 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef FT_PRINTF_H
# define FT_PRINTF_H

# include <stdarg.h>
# include "../libft.h"

typedef struct	s_print
{
	int			len;
	char		fm;
	char		fz;
	int			width;
	int			p;
	char		type;
	int			index;
}				t_print;

/*
** ft_printf.c
*/

int				ft_printf(const char *format, ...);
void			format_iter(const char *format, t_print *var, va_list ap);
void			hub_type(t_print *var, va_list ap);

/*
** parsing.c
*/

void			set_struct(t_print *pf_var);
void			set_width(const char *format, t_print *var, va_list ap);
void			set_flag(const char *format, t_print *var);
void			set_precision(const char *format, t_print *var, va_list ap);
void			set_type(const char *format, t_print *var);

/*
** layout.c
*/

int				p_r(char *str, int len, char c, int str_limit);
int				p_l(char *str, int len, char c, int str_limit);
void			fill_zero(char *str, int zero);
int				append_zero(char **str, int zero);

/*
** print_char.c
*/

void			print_pourcent(t_print *var);
void			print_c(t_print *var, va_list ap);
void			print_s(t_print *var, va_list ap);

/*
** print_xp.c
*/

char			*convert_x(char type, unsigned int nb);
void			print_x(t_print *var, va_list ap);
char			*app_p(char **str);
void			print_p(t_print *var, va_list ap);

/*
** print_du.c
*/

int				print_d_zero(char *str, int width, int len);
void			print_d(t_print *var, va_list ap);
void			print_u(t_print *var, va_list ap);

#endif
