/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_give_cmd_birth.c                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 15:39:46 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/13 22:48:53 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

int		unexpected_token_msg(char *input)
{
	if (input && input[1] && input[0] == input[1] && input[1] == '|')
		return (-2);
	ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
	if (!input)
		ft_putstr_fd("newline'\n", 2);
	else if (input[0])
	{
		ft_putchar_fd(input[0], 2);
		if (input[1] && input[0] == input[1])
			ft_putchar_fd(input[1], 2);
		ft_putstr_fd("'\n", 2);
	}
	return (-2);
}

void	get_baby_argv(t_cmd *cmd, char *input, int cmd_len)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!input)
		return ;
	while (input[i] && i < cmd_len)
	{
		if (child_len(&input[i]))
		{
			cmd->argv[j] = ft_substr(&input[i], 0, child_len(&input[i]));
			i += child_len(&input[i]);
			j++;
		}
		else if (is_rdir(input[i]))
		{
			i += get_rdir_type(NULL, &input[i]);
			i += child_len(&input[i]);
		}
		else
			i++;
	}
}

int		cmd_len(t_cmd *cmd, char *input)
{
	int	len;

	len = 0;
	while (input && input[len] && is_ifs(input[len]))
		len++;
	if (input && input[len] && (is_cmd_sep(input[len])))
		return (unexpected_token_msg(&input[len]));
	while (input && input[len] && !(is_cmd_sep(input[len])))
	{
		if (child_len(&input[len]))
		{
			cmd->argc++;
			len += child_len(&input[len]);
		}
		else if (is_rdir(input[len]))
		{
			if (get_rdir_type(NULL, &input[len]) > 0)
				len += get_baby_rdir(&cmd->rdir, &input[len]);
			else
				return (-1);
		}
		else
			len++;
	}
	return (len);
}

int		give_cmd_birth(t_cmd **cmd, char *input, char **env[])
{
	int		len;
	int		ret;

	*cmd = NULL;
//	ft_printf("\ninput dans parsing is= >%s<", input);//debug
	if (!input || !(*cmd = init_cmd()))
		return (0);
	if ((len = cmd_len(*cmd, input)) < 0)
	{
		*cmd = free_and_null_cmd(cmd);
		return (-len);
	}
	(*cmd)->argv = init_argv((*cmd)->argc);
	get_baby_argv(*cmd, input, len);
//	print_args(*cmd->argc, *cmd->argv);//debug
	if (input[len] && (input[len] == ';' || input[len] == '|'))
	{
		if (input[len] == '|')
			(*cmd)->pipe++;
		if (input[len + 1]
			&& (ret = give_cmd_birth(&((*cmd)->next), &input[len + 1], env)))
		{
//			ft_printf("\nstatus modified in parsing:%d", ret);
			edit_exit_status(env, ret);
			*cmd = free_and_null_cmd(cmd);
			return (ret);
		}
	}
	return (0);
}
