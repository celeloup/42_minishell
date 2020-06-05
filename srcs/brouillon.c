/*
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
				//ft_printf("error : %s\n", strerror(errno)); // !!!! attention error not printed because of pipe
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
		*/

		
		char **arguments = ft_split(input, ' ');
		//EXECUTION
		if (fork() == 0) //child
		{
			/*
			if (ft_strcmp("/bin/ls", arguments[0]) == 0)
			{
				//REDIRECTION TO FILE PART
				int fd = open("testredirection", O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
				if (fd == -1) //du coup arrêter execution
					ft_printf("erreur redirection : %s\n", strerror(errno));
				dup2(fd, 1);
				close(fd);
			}*/
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