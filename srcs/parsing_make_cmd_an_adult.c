/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_make_cmd_an_adult.c                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/13 20:50:32 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		words_in_baby(char *baby, char *env[])
{
	int		i;
	int		wc;
	char	*teen;

	i = 0;
	wc = 0;
	if (!(teen = make_baby_a_teen(baby, env)))
		return (0);
	if (!teen[0] || baby[ft_strlen(baby) - 1] == DOLLAR)
		i = ft_strlen(teen);
	while (teen[i] && is_ifs(teen[i]))
		i++;
	while (teen[i])
	{
		wc++;
		while (teen[i] && !is_ifs(teen[i]))
			i += go_to_next_char(&teen[i], NO);
		while (teen[i] && is_ifs(teen[i]))
			i++;
	}
	teen = free_and_null_str(&teen);
	if (wc == 0)
		wc = 1;
	return (wc);
}

int		get_adult_argc(char **baby_argv, char *env[])
{
	int		adult_argc;
	int		i;

	adult_argc = 0;
	i = 0;
	while (baby_argv[i])
	{
		adult_argc += words_in_baby(baby_argv[i], env);
		i++;
	}
	return (adult_argc);
}

int		make_cmd_an_adult(t_cmd *baby, char *env[])
{
	int		ret;
	t_cmd	*adult;

	if (!baby)
		return (0);
	adult = init_cmd();
	adult->argc = get_adult_argc(baby->argv, env);
	adult->argv = get_adult_argv(baby->argv, adult->argc, env);
//	ft_printf("\nMAKE_ADULT printarg");//debug
//	print_args(adult->argc, adult->argv);//debug
	baby->argv = free_and_null_tab(&baby->argv);
	baby->argv = copy_argv(adult->argv, adult->argc);
	baby->argc = adult->argc;
	ret = get_adult_rdir(&adult->rdir, baby->rdir, env);
	baby->rdir = free_and_null_rdir(&baby->rdir);
	baby->rdir = copy_rdir(adult->rdir);
	adult = free_and_null_cmd(&adult);
	return (ret);
}
