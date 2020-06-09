/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
<<<<<<< HEAD
/*   Updated: 2020/06/09 20:04:32 by celeloup         ###   ########.fr       */
=======
/*   Updated: 2020/06/09 19:30:34 by celeloup         ###   ########.fr       */
>>>>>>> pipes work in progress
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>
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

int		redirections(t_rd	*rd)
{
	int fd;
	while (rd)
	{
		if (rd->type == RD_IN)
		{
			fd = open(rd->value, O_RDONLY);
			if (fd == -1)
				return (-1);
			if (dup2(fd, STDIN_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
		}
		else if (rd->type == RD_OUT)
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
				return (-1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
		}
		else
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
				return (-1);
			if (dup2(fd, STDOUT_FILENO) == -1)
				return (-1);
			if (close(fd) == -1)
				return (-1);
		}
		rd = rd->next;
	}
	return(0);
}

int		exec_cmd(t_cmd *cmd, char*env[])
{
	if (redirections(cmd->rd) == -1)
		ft_printf("error redirection");
	if (is_builtins(cmd, env) == -1)
	{
		if (execve(cmd->argv[0], cmd->argv, env) == -1)
		{
			char *test = strerror(errno);
			write(2, test, ft_strlen(test));
			write(2, "\n", 1);
			exit(0);
		}
	}
	return (0);
}

int		pipe_count(t_cmd *cmd)
{
	int		nb_pipe;

	nb_pipe = 0;
	while (cmd && cmd->pipe)
	{
		nb_pipe++;
		cmd = cmd->next;
	}
	return (nb_pipe);
}

int		exec_pipes(t_cmd *cmd, char *env[])
{
	int pipefds[2];
	if (pipe(pipefds) == -1)
		ft_printf("pipe failed"); // + exit
		int pid;
	if ((pid = fork()) == -1) 
		ft_printf("fork failed"); // + exit
	else if (pid == 0) //child
	{
		close(pipefds[0]);
		dup2(pipefds[1], 1);
		close(pipefds[1]);
		exec_cmd(cmd, env);
	}
	else //parent
	{
		close(pipefds[1]);
		dup2(pipefds[0], 0);
		close(pipefds[0]);
		exec_cmd(cmd->next, env);
	}
	return (0);
}

int		exec_cmds(t_cmd *cmd, char *env[])
{
	pid_t	pid;
	int		status;
	//int		*pipes_tab;

	while (cmd)
	{	
		if ((pid = fork()) == -1)
		{
			write(2, "error fork", 11);
			exit(EXIT_FAILURE);
		}
		if (pid == 0)
		{
			exec_cmd(cmd, env);
		}
		wait(&status);
		cmd = cmd->next;
	}
	return (status);
}

t_cmd	*construct_list(char *input)
{
	t_cmd	*cmd;
	t_cmd	*cmd2;
	t_rd	*rd1;
	t_rd	*rd2;

	rd1 = malloc(sizeof(t_cmd));
	rd1->type = RD_OUT;
	rd1->value = "test";
	rd2 = malloc(sizeof(t_cmd));
	rd1->next = NULL;
	
	rd2->type = RD_OUT;
	rd2->value = "second_test";
	rd2->next = NULL;

	cmd = malloc(sizeof(t_cmd));
	cmd2 = malloc(sizeof(t_cmd));
	cmd->argv = NULL;
	//cmd->argv = ft_split(input, ' ');
	(void)input;
	cmd->pipe = 0;
	cmd->rd = rd1;
	cmd->next = NULL;
	cmd2->argv = ft_split("/usr/bin/wc Makefile", ' ');
	cmd2->pipe = 0;
	t_cmd *cmd3 = malloc(sizeof(t_cmd));
	cmd3->argv = ft_split("/bin/echo cmd3", ' ');
	cmd3->next = NULL;
	cmd->pipe = 0;
	cmd2->next = cmd3;
	return (cmd);
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
	signal(SIGTERM, signal_handler);
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
