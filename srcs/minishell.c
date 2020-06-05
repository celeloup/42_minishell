/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/05 19:10:31 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** Affiche le prompt en couleurs
** Si y'a eu erreur avant, affiche flèche en rouge, sinon vert
*/

void	prompt(int error)
{
	if (error == 1)
		ft_printf("%s➜  %sminishell %s> %s", RED, BLUE, YELLOW, END);
	else
		ft_printf("%s➜  %sminishell %s> %s", GREEN, BLUE, YELLOW, END);
}

int		is_builtins(t_cmd *cmd, char *env[])
{
	if (ft_strcmp("exit", cmd->argv[0]) == 0)
		ft_exit(cmd, env);
	else if (ft_strcmp("echo", cmd->argv[0]) == 0)
		ft_echo(cmd, env);
	else if (ft_strcmp("cd", cmd->argv[0]) == 0)
		ft_cd(cmd, env);
	else if (ft_strcmp("pwd", cmd->argv[0]) == 0)
		ft_pwd(cmd, env);
	else if (ft_strcmp("export", cmd->argv[0]) == 0)
		ft_export(cmd, env);
	else if (ft_strcmp("unset", cmd->argv[0]) == 0)
		ft_unset(cmd, env);
	else if (ft_strcmp("env", cmd->argv[0]) == 0)
		ft_env(cmd, env);
	else
		return (-1);
	return (0);
}

char	*get_env_var(char *var, char *env[])
{
	size_t	i;

	i = 0;
	while (env[i])
	{
		if (!strncmp(env[i], var + 1, ft_strlen(var + 1)))
			return (ft_strdup(ft_strrchr(env[i], '=') + 1));
		i++;
	}
	return (NULL);
}

void	exec_cmds(t_cmd *cmd, char *env[])
{
	while (cmd)
	{
		if (is_builtins(cmd, env) == 0)
		{
			cmd = cmd->next;
			continue;
		}
		else
		{
			free(cmd->argv[0]);
			cmd->argv[0] = ft_strjoin("test", "hehe");
			ft_printf("%s not a builtins\n", cmd->argv[0]);
		}
		cmd = cmd->next;
	}
}

int		main(int argc, char *argv[], char *env[])
{
	char	*input;
	t_arg	arg;
	t_cmd	*cmd;
	t_cmd	*cmd2;

	(void)argc;
	(void)argv;
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);
	init_arg(&arg);
	while (42)
	{
		prompt(0);
		get_next_line(0, &input);
		if (!input)
			control_d();
		else
			parse(input, &arg);
		cmd = malloc(sizeof(t_cmd));
		cmd2 = malloc(sizeof(t_cmd));
		cmd->argv = ft_split(input, ' ');
		cmd->next = cmd2;
		cmd2->argv = ft_split("hihihi de test", ' ');
		cmd2->next = NULL;
		ft_printf("$PATH = %s\n", get_env_var("$PWD", env));
		exec_cmds(cmd, env);
		free_arg(&arg);
	}
	free_arg(&arg);
	return (0);
}
