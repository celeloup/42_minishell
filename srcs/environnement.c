/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environnement.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 21:28:53 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/11 10:41:14 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

char	**init_env(char *env[])
{
	int	i;
	char	**new_env;

	i = 0;
	while (env[i])
		i++;
	if ((new_env = (char**)malloc(sizeof(char*) * i + 1)) == NULL)
		return (NULL);
	i = 0;
	while (env[i])
	{
		new_env[i] = ft_strdup(env[i]);
		i++;
	}
	new_env[i] = NULL;
	return (new_env);
}

void	free_env(char *env[])
{
	int i;

	i = 0;
	while (env[i])
	{
		free(env[i]);
		env[i] = NULL;
		i++;
	}
}

/**
** Adds a variable to the environnement tab.
** Var must be of format "VAR=value".
*/
int		add_env(char **env[], char *var) //ajouter check si variable existe avant ajout (et si oui, juste modifier) et check variable bon format ?
{
	int i;
	char **new_env;

	i = 0;
	while ((*env)[i])
		i++;
	if ((new_env = (char**)malloc(sizeof(char*) * i + 2)) == NULL)
		return (-1);
	i = 0;
	while ((*env)[i])
	{
		new_env[i] = ft_strdup((*env)[i]);
		i++;
	}
	new_env[i] = var;
	new_env[i + 1] = NULL;
	free_env(*env);
	*env = new_env;
	return (0);
}
/*

int		modify_env(char **env[], char *var, char *value)
{
	
}

int		remove_env(char **env[], char *var)
{
	
}
*/

void	print_env(char *env[])
{
	int i;

	i = 0;
	while (env[i])
	{
		ft_putstr(env[i]);
		ft_putchar('\n');
		i++;
	}
}