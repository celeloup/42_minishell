/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/09 20:01:32 by celeloup         ###   ########.fr       */
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
	t_cmd	*cmd_list;

	// Pour l'instant je m'en sers pas donc je les ai mute pour les flags
	//(void)env;

	//Pour comparaison (marche si args minishell idem args lancement programme)
	print_args(argc, argv);
	
	//INTERCEPTION DES SIGNAUX
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);

	cmd_list = NULL;
	while (42)
	{
		prompt(0);
		get_next_line(0, &input);
		if (!input)
			control_d();
		else if (ft_strcmp("exit", input) == 0)
			exit(EXIT_SUCCESS);
		
		// PARSING OF INPUT
		else 
		{
			ft_printf("input is : >%s<\n", input);
			cmd_list = parse_input(input, env);
			print_cmd(cmd_list, 0);
		}
		//free cmd ici ? (ft free_cmd existe dejà)
	}
	return (0);
}
