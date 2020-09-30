/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 15:39:46 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/30 09:39:18 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expanded_str(char *input, char *env[], int quote)
{
	if (!input)
		return (NULL);//vérfier ?
	else if (input[0] == BKSLASH)
		return (get_escaped_char(input, quote));
	else if (is_quote(input[0]) && !quote)
		return (get_quote(input, env));
	else if (input[0] == DOLLAR)
		return (get_var_value(get_var_name(input), env));
	else
		return (ft_substr(input, 0, 1));
}

void	get_cmd_argv(t_cmd *cmd, char *input, char *env[], int cmd_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!input)
		return ;
	//ft_printf("\nGETCMDARGVdebut\ninput is: %s", input);//debug
	while (input[i] && i < cmd_len)
	{
		if (token_len(&input[i], env, NOT_EXP))
		{
			cmd->argv[j] = get_token(&input[i], env);
			i += token_len(&input[i], env, NOT_EXP);
			j++;
		}
		else if (is_rdir(input[i]))
		{
			i += get_rdir_type(NULL, &input[i]);
			i += token_len(&input[i], env, NOT_EXP);
		}
		else
			i++;
	}
	//ft_printf("\nGETCMDARGVfin");//debug
	//print_args(cmd->argc, cmd->argv);//debug
}

int		cmd_len(t_cmd *cmd, char *input, char *env[])
{
	int	len;

	len = 0;
	while (input && input[len] && is_ifs(input[len]))
		len++;
	if (input && input[len] && (is_cmd_sep(input[len])))
		return (unexpected_token_msg(input));
	while (input && input[len] && !(is_cmd_sep(input[len])))
	{
		if (token_len(&input[len], env, NOT_EXP))
		{
			cmd->argc++;
			len += token_len(&input[len], env, NOT_EXP);
		}
		else if (is_rdir(input[len]))
		{
			if (get_rdir_type(NULL, &input[len]) > 0)
				len += get_cmd_rdir(&cmd->rdir, &input[len], env);
			else
				return (-1);
		}
		else
			len++;
	}
	return (len);
}

t_cmd	*parse_input(char *input, char *env[])
{
	t_cmd	*cmd;
	int		len;

	cmd = NULL;
	if (!(cmd = init_cmd()))
		return (NULL);
	if ((len = cmd_len(cmd, input, env)) == -1)
		return (free_cmd(cmd));
	if (input && input[len] && input[len + 1] && is_cmd_sep(input[len + 1]))
		unexpected_token_msg(&input[len]);
	cmd->argv = (char **)malloc(sizeof(char *) * (cmd->argc + 1));
	cmd->argv[cmd->argc] = NULL;
	get_cmd_argv(cmd, input, env, len);
	//print_args(cmd->argc, cmd->argv);//debug
	if (input && input[len] && (input[len] == ';' || input[len] == '|'))
	{
		if (input[len] == '|')
			cmd->pipe++;
		len++;
		cmd->next = parse_input(&input[len], env);
		if (!cmd->next)
			return (free_cmd(cmd));
	}
	return (cmd);
}
