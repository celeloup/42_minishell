/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rdir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/09 21:18:47 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		get_adult_rdir(t_rdir **adult, t_rdir *baby, char *env[])
{
	char	*teen;

	teen = NULL;
	if (baby && words_in_baby(baby->value, env) != 1)
	{
		ft_printf("\nminishell: %s: ambiguous redirect", baby->value);
		(*adult) = NULL;
		return (1);
	}
	else if (baby)
	{
		(*adult) = init_rdir();
		(*adult)->type = baby->type;
		teen = make_baby_a_teen(baby->value, env);
		(*adult)->value = get_one_adult_arg(teen, env);
		teen = free_and_null_str(&teen);
		if (baby->next)
			return (get_adult_rdir(&(*adult)->next, baby->next, env));
	}
	return (0);
}

int		get_rdir_type(t_rdir *rdir, char *input)
{
	int	i;
	int rdir_type;

	i = 0;
	if (input[0] == '<')
		rdir_type = RDIR_IN;
	else if (input[0] == '>' && input[1] && input[1] == '>')
		rdir_type = APP_RDIR_OUT;
	else
		rdir_type = RDIR_OUT;
	if (rdir_type == RDIR_IN || rdir_type == RDIR_OUT)
		i++;
	else
		i += 2;
	while (input[i] && is_ifs(input[i]))
		i++;
	if (!input[i])
		return (unexpected_token_msg(NULL));
	else if (input[i] && (is_rdir(input[i]) || is_cmd_sep(input[i])))
		return (unexpected_token_msg(&input[i]));
	if (rdir)
		rdir->type = rdir_type;
	return (i);
}

int		get_baby_rdir(t_rdir **rdir, char *input)
{
	int	i;

	if (*rdir)
	{
		while (*rdir)
			rdir = &(*rdir)->next;
	}
	i = 0;
	*rdir = init_rdir();
	if (get_rdir_type(NULL, input) > 0)
		i += get_rdir_type(*rdir, input);
	else
		return (-1);
	(*rdir)->value = ft_substr(&input[i], 0, child_len(&input[i]));
	i += child_len(&input[i]);
	return (i);
}
