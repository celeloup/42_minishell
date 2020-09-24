/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: amenadier <amenadier@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/09/23 16:25:57 by amenadier        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

/*
** The error code of an invalid identifier/name is actually 1. 
** Because of var functions uses it is set to its negative value here and reset
** to its positive value afterward
*/
	int
print_env_error(char *input, char *cmd, int error_type)
{
    ft_putstr_fd("minishell: ", 2);
    if (cmd)
    {
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

    void
unexpected_token_msg(char *input)
{
    // ft_printf("unexpected token msg input = %s\n", input);//debug a supprimer
    if (input && input[1] && input[0] == input[1] && input[1] == '|')
        return ;
    ft_putstr_fd("minishell: syntax error near unexpected token `", 2);
    if (!input)
        ft_putstr_fd("newline'\n", 2);
    else if (input[0])
    {
        ft_putchar_fd(input[0], 2);
        if (input[1] && input[0] == input[1])
            ft_putchar_fd(input[1], 2);
        ft_putstr_fd("'\n", 2);
    }
    // ft_printf("unexpected token msg input = %s\n", input);//debug a supprimer
}

    int
parsing_error(char *input, int error_type)
{
    if (error_type == UNEXPECTED_TOKEN)
        unexpected_token_msg(input);
    // ft_printf("fin de parsing error\n");
    return (-1);
}
