/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/23 12:17:07 by celeloup          #+#    #+#             */
/*   Updated: 2020/10/15 10:54:48 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <dirent.h>

char	*path_join(char *bin, char *path)
{
	char	*new_path;
	size_t	i;
	size_t	j;

	if ((new_path = malloc((ft_strlen(bin) + ft_strlen(path) + 2)
		* sizeof(char))) == NULL)
		return (NULL);
	i = 0;
	while (i < ft_strlen(bin))
	{
		new_path[i] = bin[i];
		i++;
	}
	new_path[i] = '/';
	i++;
	j = 0;
	while (j < ft_strlen(path))
	{
		new_path[i] = path[j];
		i++;
		j++;
	}
	new_path[i] = '\0';
	return (new_path);
}

char	*search_path(char **bin_tab, char *cmd)
{
	int				i;
	DIR				*dir;
	struct dirent	*entry;
	char			*cmd_name;

	i = 0;
	while (bin_tab && bin_tab[i])
	{
		if ((dir = opendir(bin_tab[i])) != NULL)
		{
			while ((entry = readdir(dir)))
			{
				if (!ft_strcmp(entry->d_name, cmd))
				{
					closedir(dir);
					cmd_name = path_join(bin_tab[i], cmd);
					free_tab(bin_tab);
					return (cmd_name);
				}
			}
			closedir(dir);
		}
		i++;
	}
	free_tab(bin_tab);
	return (NULL);
}

char	*get_cmd_path(char *cmd, char *env[])
{
	char	*path;
	char	**bin_tab;
	char	*cmd_name;

	if (ft_strchr(cmd, '/') != NULL || !(ft_strncmp(cmd, ".", 1)))
		return (cmd);
	path = get_var_value("$PATH", env);
	if (path == NULL)
		return (cmd);
	bin_tab = ft_split(path, ':');
	free(path);
	cmd_name = search_path(bin_tab, cmd);
	if (cmd_name != NULL)
		return (cmd_name);
	else
		return (cmd);
}

int		(*builtin(char *cmd_name))(t_cmd*, char***)
{
	if (ft_strcmp("exit", cmd_name) == 0)
		return (ft_exit);
	else if (ft_strcmp("echo", cmd_name) == 0)
		return (ft_echo);
	else if (ft_strcmp("cd", cmd_name) == 0)
		return (ft_cd);
	else if (ft_strcmp("pwd", cmd_name) == 0)
		return (ft_pwd);
	else if (ft_strcmp("export", cmd_name) == 0)
		return (ft_export);
	else if (ft_strcmp("unset", cmd_name) == 0)
		return (ft_unset);
	else if (ft_strcmp("env", cmd_name) == 0)
		return (ft_env);
	else
		return (NULL);
}

int		size_pipeline(t_cmd *cmd)
{
	int i;

	i = 0;
	while (cmd->pipe == 1)
	{
		i++;
		cmd = cmd->next;
	}
	i++;
	return (i);
}

void	error_msg(char *actor, char *msg)
{
	ft_putstr_fd("minishell: ", 2);
	ft_putstr_fd(actor, 2);
	ft_putstr_fd(": ", 2);
	ft_putstr_fd(msg, 2);
	ft_putchar_fd('\n', 2);
}

void	error_exit(int status, t_cmd *cmd, char *env[])
{
	free_and_null_cmd(&cmd);
	free_and_null_tab(&env);
	exit(status);
}

/*
** direction 1 = in, direction 2 = out, direction 3 = both
*/

int		redirect(t_rdir *rd, int direction, int fd, int flux)
{
	if (fd != -1 && flux != -1)
	{
		if (dup2(fd, flux) == -1)
		{
			//error_msg("dup2", strerror(errno));
			return (-1);
		}
		if (close(fd) == -1)
		{
			error_msg("close", strerror(errno));
			return (-1);
		}
	}
	while (rd)
	{
		if (rd->type == RDIR_IN && (direction == 1 || direction == 3))
		{
			fd = open(rd->value, O_RDONLY);
			if (fd == -1)
			{
				error_msg(rd->value, strerror(errno));
				return (-1);
			}
			flux = STDIN_FILENO;
		}
		else if (rd->type == RDIR_OUT && (direction == 2 || direction == 3))
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_TRUNC, 0644);
			if (fd == -1)
			{
				error_msg(rd->value, strerror(errno));
				return (-1);
			}
			flux = STDOUT_FILENO;
		}
		else if (direction == 1 || direction == 3)
		{
			fd = open(rd->value, O_WRONLY | O_CREAT | O_APPEND, 0644);
			if (fd == -1)
			{
				error_msg(rd->value, strerror(errno));
				return (-1);
			}
			flux = STDOUT_FILENO;
		}
		if (dup2(fd, flux) == -1)
		{
			//error_msg("dup2", strerror(errno));
			return (-1);
		}
		if (close(fd) == -1)
		{
			error_msg("close", strerror(errno));
			return (-1);
		}
		rd = rd->next;
	}
	return (0);
}

void 	init_stuff(t_cmd *cmd)
{
	while(cmd)
	{
		cmd->status = -1;
		cmd->pid = -1;
		cmd = cmd->next;
	}
}

void	wait_for_stuff(t_cmd *cmd)
{
	while (cmd)
	{
		waitpid(cmd->pid, &cmd->status, 0);
		cmd = cmd->next;
	}
}

int		get_last_status(t_cmd *cmd)
{
	int status;

	while (cmd->pipe == 1)
	{
		cmd = cmd->next;
		status = cmd->status;
	}
	return (status);
}

int		get_status(char **env[])
{
	char *tmp;
	int status;

	tmp = get_var_value("$?", *env);
	status = atoi(tmp);
	tmp = free_and_null_str(&tmp);
	return (status);
}

int		is_binary_file(char *file_name)
{
	DIR				*dir;
	struct dirent	*entry;

	if ((dir = opendir(".")) != NULL)
	{
		while ((entry = readdir(dir)))
		{
			if (!ft_strcmp(entry->d_name, file_name))
			{
				closedir(dir);
				return (1);
			}
		}
		closedir(dir);
	}
	return (0);
}

int		exec_cmds(t_cmd *cmd, char **env[])
{
	int status;
	pid_t pid;
	int fdpipe[2];
	int pipe_length;
	int tmpin;
	int tmpout;
	int i;
	t_cmd *first;

	status = get_status(env);
	first = cmd;
	init_stuff(cmd);
	while (cmd)
	{
		while(make_cmd_an_adult(cmd, env))//change le status dans env mais pas ici
			cmd = cmd->next;
		tmpin = dup(STDIN_FILENO); //a proteger
		tmpout = dup(STDOUT_FILENO); // a proteger
		//if (!cmd || !cmd->argv || !cmd->argv[0]) // SUPPR POUR CAS OU REDIR SEULE
		//	return (0);//vérifier le cas du cmd->next
		if (cmd->argv && cmd->argv[0] && builtin(cmd->argv[0]) != NULL && cmd->pipe == 0)
		{
			if (redirect(cmd->rdir, 3, -1, -1) != -1)
				status = (*builtin(cmd->argv[0]))(cmd, env);
			else
				status = 1;
			//ft_printf("\ninside exec cmd status is %d", status);
		}
		else
		{
			if (cmd->pipe == 1)
			{
				pipe_length = size_pipeline(cmd);
				//fdpipe[0] = dup(tmpin); // a proteger
				i = 0;
				while (i < pipe_length)
				{
					pipe(fdpipe); // a proteger
					pid = fork(); //a proteger
					cmd->pid = pid;
					if (pid == 0)
					{
						if (close(fdpipe[0]) == -1)
							ft_printf("error close fdpipe0");//a proteger
						if (cmd->pipe != 0)
						{
							if (redirect(cmd->rdir, 2, fdpipe[1], STDOUT_FILENO) == -1)
								error_exit(1, first, *env);
						}
						else
						{
							if (redirect(cmd->rdir, 2, tmpout, STDOUT_FILENO) == -1)
								error_exit(1, first, *env);
							close(fdpipe[1]); //a proteger
						}
						if (builtin(cmd->argv[0]) != NULL)
						{
							status = (*builtin(cmd->argv[0]))(cmd, env);
							if (status != -1)
								error_exit(status, first, *env);
							else
								error_exit(get_status(env), first, *env);
						}
						else
						{
							if (!is_binary_file(cmd->argv[0]))
								execve(get_cmd_path(cmd->argv[0], *env), cmd->argv, *env);
							if (ft_strncmp(cmd->argv[0], "./", 2) == 0)
							{
								error_msg(cmd->argv[0], "Permission denied");
								error_exit(126, first, *env);
							}
							else if (!var_is_set(env, "PATH"))
							{
								error_msg(cmd->argv[0], "No such file or directory");
								error_exit(127, first, *env);
							}
							else
							{
								error_msg(cmd->argv[0], "command not found");
								error_exit(127, first, *env);
							}
						}
					}
					else
					{
						close(fdpipe[1]); //a proteger
						if (redirect(cmd->rdir, 1, fdpipe[0], STDIN_FILENO) == -1)
							error_exit(1, first, *env);
					}
					if (cmd->pipe == 1)
					{
						cmd = cmd->next;
						while (make_cmd_an_adult(cmd, env))
							cmd = cmd->next;
					}
					i++;
				}
				wait_for_stuff(first);
				status = cmd->status; //get_last_status(first);
				//ft_printf("status = %d\ncmd = %s\n", status, cmd->argv[0]);
			}
			else
			{
				if (redirect(cmd->rdir, 3, -1, -1) == -1)
					status = 1;
				if (status != 1 && cmd->argv && cmd->argv[0] && cmd->argv[0][0] && (pid = fork()) == 0)
				{
					//ft_printf("%s\n", get_cmd_path(cmd->argv[0], *env));
					if (!is_binary_file(cmd->argv[0]))
						execve(get_cmd_path(cmd->argv[0], *env), cmd->argv, *env);
					if (ft_strncmp(cmd->argv[0], "./", 2) == 0)
					{
						error_msg(cmd->argv[0], "Permission denied");
						error_exit(126, first, *env);
					}
					else if (!var_is_set(env, "PATH"))
					{
						error_msg(cmd->argv[0], "No such file or directory");
						error_exit(127, first, *env);
					}
					else
					{
						error_msg(cmd->argv[0], "command not found");
						error_exit(127, first, *env);
					}
				}
				else
					waitpid(pid, &status, 0);
			}
		}
		status = ((status != 1 && status != 2) ? WEXITSTATUS(status) : status);
		cmd = cmd->next;
		if (status != 255)
			edit_exit_status(env, status);
		redirect(NULL, 0, tmpin, STDIN_FILENO); //a proteger ?
		redirect(NULL, 0, tmpout, STDOUT_FILENO); //a proteger ?
	}
	return (status);
}
