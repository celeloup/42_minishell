/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   expand_args.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/06 19:34:19 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
int		words_in_expanded_token(char *arg, char *env[])
{
	int		i;
	int		wc;
	
	i = 0;
	wc = 0;
	if (!arg || (arg && !arg[0]))
		return (1);
	while (arg && arg[i])
	{
		wc++;
		i += token_len(&arg[i], env, NOT_EXP);
	}
	return (wc);
}

int		get_new_argc(t_cmd *cmd, char *env[])
{
	int		argc;
	int		i;
	int		j;
	char	*tmp;

	argc = 0;
	i = 0;
	tmp = NULL;
	while (cmd->argv[i])
	{
		j = 0;
		while (cmd->argv[i][j])
		{
			tmp = free_and_null(&tmp);
			tmp = get_expanded_token(&cmd->argv[i][j], env);
			argc += words_in_expanded_token(tmp, env);
			j += token_len(&cmd->argv[i][j], env, NOT_EXP);
		}
		i++;
	}
	tmp = free_and_null(&tmp);
	return (argc);
}

char	*get_new_argv(char *arg_list)
{
	int		len;
	char	*new_argv;

	len = arg_list_len(arg_list, NO);
	ft_printf("\n arg list len is : %d", len);
	new_argv = NULL;
	if (!(new_argv = (char*)malloc(sizeof(char) * (len + 1))))
		return (NULL);
	new_argv = ft_strncpy(new_argv, arg_list, len);
	new_argv[len] = '\0';
	return (new_argv);
}

void	pass_out_args(char **argv, char *tmp, )
{
	
}

void	get_new_args(t_cmd *new, t_cmd *cmd, char *env[])
{
	int		i;
	int		j;
	int		k;
	char	*tmp;

	i = 0;
	k = 0;
	tmp = NULL;
	while (cmd->argv[i])
	{
		j = 0;
		while (cmd->argv[i][j])
		{
			tmp = free_and_null(&tmp);
			tmp = get_expanded_token(&cmd->argv[i][j], env);
			pass_out_args(new->argv, tmp, &k);
			while (tmp)
			argc += words_in_expanded_token(tmp, env);
			new->argv[k] = get_expanded_token(&cmd->argv[i][j], env);
			k++;
			j += token_len(&cmd->argv[i][j], env, NOT_EXP);
		}
		i++;
	}
	tmp = free_and_null(&tmp);
	return (argc);
}

char	**argv_dup(char	**argv, int argc)
{
	char	**cpy;
	int		i;
	
	cpy = (char **)malloc(sizeof(char *) * (argc + 1));
	i = 0;
	while (i < argc)
	{
		cpy[i] = ft_strdup(argv[i]);
		i++;
	}
	cpy[i] = NULL;
	return (cpy);
}

t_rdir	*copy_rdir(t_rdir *rdir)
{
	t_rdir	*cpy;

	if (!rdir)
		return (NULL);
	cpy = init_rdir();
	cpy->next = copy_rdir(rdir->next);
	cpy->type = rdir->type;
	cpy->value = ft_strdup(rdir->value);
	return (cpy);
}

t_cmd	*copy_cmd(t_cmd *cmd)
{
	t_cmd	*cpy;

	if (!cmd)
		return (NULL);
	cpy = init_cmd();
	cpy->argc = cmd->argc;
	cpy->argv = argv_dup(cmd->argv, cmd->argc);
	cpy->pipe = cmd->pipe;
	cpy->rdir = copy_rdir(cmd->rdir);
	cpy->next = copy_cmd(cmd->next);
	return (cpy);
}


t_cmd	*expand_args_in_cmd(t_cmd *cmd, char *env[])
{
	t_cmd	*new;
	int		i;
	int		len;

	if (!cmd)
		return (NULL);
	new = init_cmd();
	i = 0;
	len = 0;
	ft_putstr_fd("\nARGS get_var_in_cmd debut CMD", 1);//debug
	print_args(cmd->argc, cmd->argv);//degug
	new->argc = get_new_argc(cmd, env);
	ft_printf("\nNEW ARGC IS : %d", new->argc);//debug
	new->argv = init_argv(new->argc);
	get_new_args(new, cmd, env);
	new->next = copy_cmd(cmd->next);
	new->pipe = (cmd->pipe);
	new->rdir = copy_rdir(cmd->rdir);
	ft_putstr_fd("\nARGS get_var_in_cmd fin NEW", 1);
	print_args(new->argc, new->argv);//degug
	ft_printf("\n\n");//debug
	return (new);
}
*/

void	expand_args_in_cmd(t_cmd *cmd, char *env[])
{
	int		i;
	char	*tmp;
	
	i = 0;
	tmp = NULL;
	while(cmd->argv && cmd->argv[i])
	{
		tmp = free_and_null(&tmp);
		tmp = get_expanded_token(cmd->argv[i], env);
		cmd->argv[i] = free_and_null(&cmd->argv[i]);
		cmd->argv[i] = ft_strdup(tmp);
		i++;
	}
	tmp = free_and_null(&tmp);
}
