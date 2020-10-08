/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/08/04 15:39:46 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/08 20:34:40 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

void	get_first_cmd_argv(t_cmd *cmd, char *input, int cmd_len)
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
		if ((len = token_len(&input[i], NULL, NOT_EXP)))
		{
			
			cmd->argv[j] = get_not_expanded_token(&input[i]);
			ft_printf("\nin get_cmd_argv, ARGV = %s", cmd->argv[j]);
			i += len;
			j++;
		}
		else if (is_rdir(input[i]))
		{
			i += get_rdir_type(NULL, &input[i]);
			i += token_len(&input[i], NULL, NOT_EXP);
		}
		else
			i++;
	}
//	if (tmp)//debug
//		free(tmp);//deubg
}

int		cmd_len(t_cmd *cmd, char *input)
{
	int	len;
	int	save;

	len = 0;
	save = 0;
	while (input && input[len] && is_ifs(input[len]))
		len++;
	if (input && input[len] && (is_cmd_sep(input[len])))
		return (unexpected_token_msg(&input[len]));
	while (input && input[len] && !(is_cmd_sep(input[len])))
	{
		ft_printf("\ncmd_len len =%d", len);
		if (token_len(&input[len], NULL, NOT_EXP))
		{
			cmd->argc++;
			len += token_len(&input[len], NULL, NOT_EXP);
		}
		else if (is_rdir(input[len]))
		{
			if (get_rdir_type(NULL, &input[len]) > 0)
				len += get_cmd_rdir(&cmd->rdir, &input[len], NULL);
			else
				return (-1);
		}
		else
			len++;
		if (input[len] && input[len] == ';')
		{
			save = len;
			len++;
			while (input[len] && is_ifs(input[len]))
				len++;
			if (input[len] && is_rdir(input[len]))
			{
				if (get_rdir_type(NULL, &input[len]) > 0)
					len += get_cmd_rdir(&cmd->rdir, &input[len], NULL);
				else
					return (-1);
			}
			else
				len = save;
		}
	}
	return (len);
}

t_cmd	*parse_input(char *input)
{
	t_cmd	*cmd;
	int		len;

	cmd = NULL;
	if (!(cmd = init_cmd()) || !input)
		return (NULL);
	ft_printf("\nparse input 0");
	if ((len = cmd_len(cmd, input)) == -1)
		return (free_and_null_cmd(&cmd));
//	if (input && input[len] && input[len + 1] && is_cmd_sep(input[len + 1]))
//		unexpected_token_msg(&input[len]);
	cmd->argv = init_argv(cmd->argc);
	ft_printf("\nparse input");
	get_first_cmd_argv(cmd, input, len);
	ft_putstr_fd("\nARGS parse :", 1);
	print_args(cmd->argc, cmd->argv);//debug
	if (input && input[len] && (input[len] == ';' || input[len] == '|'))
	{
		if (input[len] == '|')
			cmd->pipe++;
		len++;
		cmd->next = parse_input(&input[len]);
		if (!cmd->next)
			return (free_and_null_cmd(&cmd));
	}
	ft_putstr_fd("\nEND OF FIRST PARSING\n\n", 1);
	return (cmd);
}
