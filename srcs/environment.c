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

	int
env_len(char *env[])
{
	int		i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

	char**
env_ncpy(char *dest[], char *src[], int start, int n)
{
	int		i;

	i = 0;
	if (!dest || !src)
		return (NULL);
	while (src[i] && i < n)
	{
		dest[start] = ft_strdup(src[i]);
		start++;
		i++;
	}
	while (i < n)
	{
		dest[start] = NULL;
		start++;
		i++;
	}
	return (dest);
}

	char**
env_dup(char *env[])
{
	int		i;
	char	**new_env;

	i = env_len(env);
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

	char**
free_env(char **env[])
{
	int		i;

	i = 0;
	while ((*env) && (*env)[i])
	{
		free((*env)[i]);
		env[i] = NULL;
		i++;
	}
	if (*env)
		free(*env);
	*env = NULL;
	return (NULL);
}

/**
** Adds a variable to the environment tab.
** Var must be of format "VAR=value". VAR is lower or upper case or digit
*/
	int
var_is_valid(char *var)
{
	int		i;

	i = -1;
	if (!var || !var[0])
		ft_putstr_fd("env var to add : is null or empty", 2);
	else if (!ft_isalnum(var[0]))//changer les msg d'erreur
		ft_putstr_fd("env var to add : is not alphanumeric format", 2);
	else
	{
		i = 0;
		while (var[i] && ft_isalnum(var[i]))
			i++;
		if (!var[i])
			ft_putstr_fd("env var to add : missing '=' after var name", 2);
		else if (var[i] != '=')
			ft_putstr_fd("env var to add : is not 'VAR=' format", 2);
		else
			return (i);// retourne la pos de égal
	}
	return (-1);
}

	int
var_is_set(char **env[], char *var)
{
	char	*tmp;
	char	*to_check;
	int		ret;

	tmp = ft_strjoin("$", var);
	to_check = get_var_name(tmp);
	if (tmp)
		free(tmp);
	tmp = get_var_value(to_check, *env);
	if (to_check)
		free(to_check);
	to_check = NULL;
	if (!tmp)
		return (0);
	else if (!ft_strcmp(tmp, ""))
		ret = 0;
	else
		ret = 1;
	free(tmp);
	tmp = NULL;
	return (ret);
}

	int
edit_var(char **env[], char *var)
{
	int	name_len;
	int	i;

	if (var_is_valid(var) > 0)
		name_len = var_is_valid(var);
	else
		return (-1);
	i = 0;
	while ((*env)[i])
	{
		if (!strncmp((*env)[i], var, name_len))
		{
			free((*env)[i]);
			(*env)[i] = ft_strdup(var);//free var * si malloquée
			return (0);
		}
		i++;
	}
	return (-1);
}

	int
add_var(char **env[], char *var)
{
	int		i;
	char	**new_env;

	if (var_is_valid(var) < 0)
		return (-1);
	if (var_is_set(env, var))// vérifier qu'il fait bien ça
		return (edit_var(env, var));
	i = 0;
	while (env && *env && (*env)[i])
		i++;
	if ((new_env = (char**)malloc(sizeof(char*) * i + 2)) == NULL)
		return (-1);
	new_env = env_ncpy(new_env, *env, 0, i);
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	if (env && *env)
		*env = free_env(env);
	*env = env_dup(new_env);
	new_env = free_env(&new_env);
	return (0);
}

	int
remove_var(char **env[], char *var)
{
	char	**new_env;
	int		name_len;
	int		i;
	
	if ((name_len = var_is_valid(var)) <= 0)
		return (-1);
	i = 0;
	while ((*env)[i] && !strncmp((*env)[i], var, name_len))
		i++;
	if (!(*env)[i])
	{
		ft_putstr_fd("var to remove does not exist", 2);//a modifier
		return (-1);
	}
	if ((new_env = (char**)malloc(sizeof(char*) * env_len(*env))) == NULL)
		return (-1);
	new_env[env_len(*env)] = NULL;
	new_env = env_ncpy(new_env, *env, 0, i);
	new_env = env_ncpy(new_env, *env + i, i, env_len(*env) - i - 1);
	*env = free_env(env);
	*env = env_dup(new_env);
	new_env = free_env(&new_env);
	return (0);
}

	void
print_env(char *env[])
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