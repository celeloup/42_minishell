/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_rdir.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/09/23 17:03:31 by amenadier         #+#    #+#             */
/*   Updated: 2020/10/08 11:54:43 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

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

int		get_rdir_value(t_rdir *rdir, char *input, char *env[])
{
	// cas d'erreur si token_len = 0 ?
	if (!rdir)// a confirmer
		return (0);// a confirmer
	if (rdir && !env)
		rdir->value = get_not_expanded_token(input);
	else if (rdir)
		rdir->value = get_expanded_token(input, env);//achtung vérifier qu'on lui envoie bien un input avec les var expanded already
	if (!env)
		return (token_len(input, env, NOT_EXP));
	else
		return (token_len(input, env, EXP));
}

int		get_cmd_rdir(t_rdir **rdir, char *input, char *env[])
{
	int		i;

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
	i += get_rdir_value(*rdir, &input[i], env);
	return (i);
}
