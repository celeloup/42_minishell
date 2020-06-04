/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: celeloup <celeloup@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/05/28 09:45:30 by celeloup          #+#    #+#             */
/*   Updated: 2020/05/28 14:53:48 by celeloup         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/src/libft.h"
# include <signal.h>

# define GREEN "\033[1;32m"
# define BLUE "\033[1;36m"
# define YELLOW "\033[0;33m"
# define RED "\033[1;31m"
# define END "\033[0m"

# define SPACE      32
# define TAB        9
# define NEWLINE    10

typedef struct s_cmd t_cmd;

typedef	struct		s_cmd{
	char			**argv;
	int				argc;
	t_cmd			*next;
}					t_cmd;

void	fonction_test_celia();

/* parsing.c */
t_cmd			*parse_input(char *input);

/* check.c */
void			print_args(int argc, char **argv, char *str);
void			print_cmd(t_cmd *list);

/* free.c */
void			free_cmd_list(t_cmd *cmd_list);

/* init.c */
t_cmd			*init_cmd_list(int count);

#endif