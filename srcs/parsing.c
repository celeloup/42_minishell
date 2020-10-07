/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 15:39:46 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/07 14:55:52 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

char	*expanded_str(char *input, char *env[], int quote, int edges)
{
	char	*var_name;
	char	*var_value;

	var_name = NULL;
	var_value = NULL;
	if (!input)
		return (NULL);//vérfier ?
	else if (input[0] == BKSLASH)
		return (get_escaped_char(input, quote));
	else if (is_quote(input[0]) && !quote)
		return (get_quote(input, env));
	else if (input[0] == DOLLAR)
	{
		var_name = get_var_name(input);
		var_value = get_var_value(var_name, env, quote, edges);
		if (var_name)
			free(var_name);
		return (var_value);
	}
	else
		return (ft_substr(input, 0, len_after_char(input, env, quote, NOT_EXP)));
}

void	get_cmd_argv(t_cmd *cmd, char *input, char *env[], int cmd_len)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	if (!input)
		return ;
	//ft_printf("\nGETCMDARGVdebut\ninput is: %s", input);//debug
	while (input[i] && i < cmd_len)
	{
		if ((len = token_len(&input[i], env, NOT_EXP)))
		{
			
			cmd->argv[j] = get_token(&input[i], env);
			ft_printf("\nin get_cmd_argv, ARGV = %s", cmd->argv[j]);
			i += len;
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
//	if (tmp)//debug
//		free(tmp);//deubg
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
		ft_printf("\ncmd_len len =%d", len);
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
	if (!(cmd = init_cmd()) || !input)
		return (NULL);
	ft_printf("\nparse input 0");
	if ((len = cmd_len(cmd, input, env)) == -1)
		return (free_cmd(cmd));
	if (input && input[len] && input[len + 1] && is_cmd_sep(input[len + 1]))
		unexpected_token_msg(&input[len]);
	cmd->argv = init_argv(cmd->argc);
	ft_printf("\nparse input");
	get_cmd_argv(cmd, input, env, len);
	ft_putstr_fd("\nARGS parse :", 1);
	print_args(cmd->argc, cmd->argv);//debug
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
