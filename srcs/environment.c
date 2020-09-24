/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   environment.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 21:28:53 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/24 11:03:15 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"
#include <stdio.h>

int		env_len(char *env[])
{
	int		i;

	i = 0;
	while (env[i])
		i++;
	return (i);
}

char	**env_ncpy(char *dest[], char *src[], int start, int n)
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

char	**env_dup(char *env[])
{
	int		i;
	char	**new_env;

	i = env_len(env);
	if ((new_env = malloc((i + 1) * sizeof(char*))) == NULL)
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
	int		i;

	i = 0;
	while (env && env[i])
	{
		free(env[i]);
		env[i] = NULL;
		i++;
	}
	if (env)
		free(env);
	env = NULL;
}


/*
** If the var format is valid, var_is_valid returns '=' index, else 0 or -errno;
*/
int		var_is_valid(char *var, char *cmd, int value_expected)
{
	if (is_not_name(var) < 0 || (is_not_name(var) && !value_expected))
		return (print_env_error(var, cmd, INVALID_NAME));
	else if (is_not_name(var) && value_expected && var[is_not_name(var)] != '=')
		return (print_env_error(var, cmd, INVALID_NAME));
	else if (is_not_name(var) && value_expected && var[is_not_name(var)] == '=')
		return (is_not_name(var));// retourne la pos de égal
	else if (is_name(var) && value_expected)
		return (0);//cas pour export 'VAR' sans egal 
	else
		return (is_name(var));
}

int		var_is_set(char **env[], char *var)
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

int		edit_var(char **env[], char *cmd, char *var)
{
	int	name_len;
	int	i;

	if (var_is_valid(var, NULL, YES) > 0)
		name_len = var_is_valid(var, NULL, YES);
	else
		return (-var_is_valid(var, cmd, YES));
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
	return (1);
}

/**
** Adds a variable to the environment tab.
** Var must be of format "VAR=value". VAR is lower or upper case or digit
*/
int		add_var(char **env[], char *cmd, char *var)
{
	int		i;
	char	**new_env;

	if (var_is_valid(var, NULL, YES) <= 0)
		return (-(var_is_valid(var, cmd, YES)));
	if (var_is_set(env, var))
		return (edit_var(env, cmd, var));
	i = 0;
	while ((*env)[i])
		i++;
	if ((new_env = (char**)malloc(sizeof(char*) * (i + 2))) == NULL)
		return (EXIT_FAILURE);
	new_env = env_ncpy(new_env, *env, 0, i);
	new_env[i] = ft_strdup(var);
	new_env[i + 1] = NULL;
	if (*env)
		free_env(*env);
	*env = env_dup(new_env);
	free_env(new_env);
	return (EXIT_SUCCESS);
}

/*
** remove_var returns 0 if var does is not already in the environment because
** unset does not consider this as an error (and therefore returns 0 too)
*/
int		remove_var(char **env[], char *cmd, char *var, int value_expected)
{
	char	**new_env;
	int		name_len;
	int		i;
	
	if ((name_len = var_is_valid(var, NULL, value_expected)) <= 0)
		return (-var_is_valid(var, cmd, value_expected));
	i = 0;
	while ((*env)[i] && (ft_strncmp((*env)[i], var, name_len)
		|| ((*env)[i])[name_len] != '='))
		i++;
	if (!(*env)[i])
		return (EXIT_SUCCESS);
	if ((new_env = (char**)malloc(sizeof(char*) * env_len(*env))) == NULL)
		return (EXIT_FAILURE);
	new_env = env_ncpy(new_env, *env, 0, i);
	new_env = env_ncpy(new_env, &((*env)[i + 1]), i, env_len(*env) - i - 1);
	new_env[env_len(*env) - 1] = NULL;
	free_env(*env);
	*env = env_dup(new_env);
	free_env(new_env);
	return (EXIT_SUCCESS);
}

void	print_env_export(char *var)
{
	int	in_quote;
	int	i;

	in_quote = 0;
	i = 0;
	ft_putstr_fd("declare -x ", 1);// + rajouter les "" a l'interieur
	while (var[i])
	{
		ft_putchar_fd(var[i], 1);
		if (var[i] == '=' && !in_quote)
		{
			ft_putchar_fd('\"', 1);
			in_quote++;
		}
		i++;
	}
	ft_putchar_fd('\"', 1);
}

void	print_env(char *env[], int option)
{
	int i;

	i = 0;
	while (env[i])
	{
		if (option == EXP)
			print_env_export(env[i]);
		else
			ft_putstr_fd(env[i], 1);
		ft_putchar_fd('\n', 1);
		i++;
	}
}
