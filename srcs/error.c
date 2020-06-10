/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:08 by celeloup          #+#    #+#             */
/*   Updated: 2020/06/09 21:22:06 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/minishell.h"

    void
unexpected_token_msg(char *input)
{//verifier le \n a la fin
    ft_putstr_fd("bash: syntax error near unexpected token `", 2);
    if (!input)
        ft_putstr_fd("newline'\n", 2);
    else if (input[0])
    {
        ft_putchar_fd(input[0], 2);
        if (input[1] && input[0] == input[1])
            ft_putchar_fd(input[1], 2);
        ft_putstr_fd("'\n", 2);
    }
}

    int
parsing_error(char *input, int error_type)
{
    if (error_type == UNEXPECTED_TOKEN)
        unexpected_token_msg(input);
    //if (cmd)
    //   free_cmd(cmd);
    //cmd = NULL;
    return (-1);
}