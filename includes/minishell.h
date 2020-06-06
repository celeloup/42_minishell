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

# define SPACE      	32
# define TAB        	9
# define NEWLINE    	10
# define IFS(x)			(x == SPACE || x == TAB || x == NEWLINE)
# define BKSLASH		92
# define SINGLE_QUOTE	39
# define DOUBLE_QUOTE	34
# define QUOTE(x)		(x == SINGLE_QUOTE || x == DOUBLE_QUOTE)
# define ESCAPE_CHAR(x)	(x == 'a' || x == 'b' || x == 'e' || x == 'f' || x == 'n' || x == 'r' || x == 't' || x == 'v' || x == BACKSLASH || x == 'x' || x == 'c')
# define RD(x)			(x == '>' || x == '<')
# define SEPARATOR(x)	(x == '|' || x == ';')

# define RD_IN			1
# define RD_OUT			2
# define APP_RD_OUT		3

typedef	struct		s_rd{
	int				type;
	char			*value;
	struct s_rd		*next;
}					t_rd;

typedef	struct		s_cmd{
	char			**argv;
	int				argc;
	t_rd			*rd;
	int				pipe;
	struct s_cmd	*next;
}					t_cmd;

void	fonction_test_celia();

/* parsing.c */
t_cmd			*parse_input(char *input);

/* debug.c */
void			print_args(int argc, char **argv);
void			print_cmd(t_cmd *cmd, int i);

/* free.c */
void			free_cmd_list(t_cmd *cmd_list);

/* init.c */
t_cmd			*init_cmd(char *input);

#endif