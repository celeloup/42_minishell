/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/04 22:28:15 by celeloup         ###   ########.fr       */
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

int		main(int argc, char *argv[], char *env[])
{
	char	*input;
	t_arg	arg;
	
	(void)argc;
	(void)argv;

	//Pour comparaison (marche si args minishell idem args lancement programme)
	//print_args(argc, argv, "Real ");
	
	//INTERCEPTION DES SIGNAUX
	signal(SIGINT, control_c);
	signal(SIGQUIT, control_slash);

	//Pour le moment parse en dehors de boucle et GNL
	//=> permet de vérifier que argc de notre minishell = argc du vrai minishell
	init_arg(&arg);
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
			//ft_printf("input is : >%s<\n", input);
			parse(input, &arg);
			//print_args(arg.count, arg.value, "(Our) mini");
		}
		
		//PIPE TEST    cat a | /usr/bin/wc
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
			char *arguments[] = {"cat", "Makefile", NULL};
			if (execve(arguments[0], arguments, env) == -1) 
			{
				write(2, "error child exec", 17);
				ft_printf("error : %s\n", strerror(errno)); // !!!! attention error not printed because of pipe
			}
			return(1);
		}
		else //parent
		{
			close(pipefds[1]);
			dup2(pipefds[0], 0);
			close(pipefds[0]);
			char *arguments[] = {"/usr/bin/wc", NULL};
			if (execve(arguments[0], arguments, env) == -1)
				ft_printf("error : %s\n", strerror(errno));
			return(1);
		}
		

		/*
		char **arguments = ft_split(input, ' ');
		//EXECUTION
		if (fork() == 0) //child
		{
			if (ft_strcmp("/bin/ls", arguments[0]) == 0)
			{
				//REDIRECTION TO FILE PART
				int fd = open("testredirection", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
				if (fd == -1) //du coup arrêter execution
					ft_printf("erreur redirection : %s\n", strerror(errno));
				dup2(fd, 1);
				close(fd);
			}
			if (execve(arguments[0], arguments, env) == -1)
			{
				char *test = strerror(errno);
				ft_printf("error : %s\n", test);
			}
			return (0);
		}
		else //parent
		{
			wait(NULL);
		}
		*/
		free_arg(&arg);
	}
	free_arg(&arg);
	return (0);
}
