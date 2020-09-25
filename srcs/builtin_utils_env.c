/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin_utils_env.c                                :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: user42 <user42@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/06/10 21:28:53 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/25 15:09:13 by user42           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** The error code of an invalid identifier/name is actually 1.
** Because of var functions uses it is set to its negative value here and reset
** to its positive value afterward
*/

int		print_env_error(char *input, char *cmd, int error_type)
{
	if (cmd)
	{
		ft_putstr_fd("minishell: ", 2);
		ft_putstr_fd(cmd, 2);
		ft_putstr_fd(": ", 2);
	}
	if (input && cmd && error_type == INVALID_NAME)
	{
		ft_putstr_fd("`", 2);
		ft_putstr_fd(input, 2);
		ft_putstr_fd("': not a valid identifier\n", 2);
		return (-1);
	}
	else if (cmd && error_type)
	{
		ft_putstr_fd(strerror(error_type), 2);
	}
	return (error_type);
}

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
