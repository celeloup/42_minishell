/*******************
//HEADER
****/

#include "../includes/minishell.h"

//IFS = Internal Field Separator

/*
	static int
tokens_count(char *input)
{
	int	single_quote;
	int	double_quote;
	int	count;
	int	i;

	i = 0;
	single_quote = 0;
	double_quote = 0;
	count = 0;
	if (!is_IFS(input[0]) && input[0] != '\0')
		count++;
	while (input[i])
	{
		if (!(double_quote % 2) && input[i] == SINGLE_QUOTE)
			single_quote++;
		if (!(single_quote % 2) && input[i] == DOUBLE_QUOTE)
			double_quote++;
		if ((!(single_quote % 2) && !(double_quote % 2))// a changer
			&& (is_IFS(input[i]) && input[i + 1] && !is_IFS(input[i + 1])))
			count++;
		i++;
	}
	return (count);
}

	int
token_len(char *input)
{
	int	len;
	int	single_quote;
	int	double_quote;

	len = 0;
	single_quote = 0;
	double_quote = 0;
	while (input[len] && input[len] == ';')
		len++;
	while (input[len] && input[0] != ';'
		&& (!is_IFS(input[len]) || (single_quote % 2) || (double_quote % 2)))
	{
		if (!(double_quote % 2) && input[len] == SINGLE_QUOTE)
			single_quote++;
		if (!(single_quote % 2) && input[len] == DOUBLE_QUOTE)
			double_quote++;
		if ((!(single_quote % 2) || !(double_quote % 2)) && input[len] == ';')
			break;
		len++;
	}
	return (len);
}

	static void
fill_tokens(char **tokens, char *input)
{
	int i;
	int j;
	int	k;
	int len;

	i = 0;
	j = 0;
	while (input[i])
	{
		if (!is_IFS(input[i]))
		{
			len = token_len(&input[i]);
			tokens[j] = (char*)malloc(sizeof(char) * (len + 1));
			k = 0;
			while (input[i] && k < len)
				tokens[j][k++] = input[i++];
			tokens[j++][len] = '\0';
		}
		else
			i++;
	}
}

	char**			
split_input(char *input)
{
	char	**tokens;
	int		count;

	if (!input)
		return (NULL);
	count = tokens_count(input);
	if (!(tokens = (char **)malloc(sizeof(char*) * (count + 1))))
		return (NULL);
	tokens[count] = NULL;
	fill_tokens(tokens, input);
	return (tokens);
}

	int
cmd_count(char **tokens)
{
	int	i;
	int	count;

	i = 0;
	count = 0;
	if (tokens[0] && ft_strcmp(tokens[0], ";"))
		count++;
	while (tokens[i])
	{
		if (!ft_strcmp(tokens[i], ";") 
			&& tokens[i + 1] && ft_strcmp(tokens[i + 1], ";"))
			count++;
		i++;
	}
	return (count);
}
	
	t_cmd*
cmd_list(char **tokens)
{
	t_cmd	*list = NULL;
	int		i;

	if (!tokens)
		return (NULL);
	if (!(list = malloc(sizeof(t_cmd))))
		return (NULL);
	list->argc = 0;
	while (tokens[list->argc] && ft_strcmp(tokens[list->argc], ";"))
			list->argc++;
	list->next = NULL;
	if (!(list->argv = (char **)malloc(sizeof(char*) * (list->argc + 1))))
		return (NULL);
	list->argv[list->argc] = NULL;
	i = 0;
	while (i < list->argc)
	{
		list->argv[i] = ft_strdup(tokens[i]);
		i++;
	}
	while (tokens[i] && !ft_strcmp(tokens[i], ";"))
		i++;
	if (tokens[i])
		list->next = cmd_list(&tokens[i]);
	return (list);
}
	
	void
check_tokens(char **tokens)
{
	(void)tokens;// A COMPLETER AVEC LES CAS D'ERREUR
}

	char**
deal_quotes(char **tokens)
{
	int	i;

	i = 0;
	while (tokens[i])
	{

		i++;
	}
	return (tokens);
}
*/


	int
quote_len(char *input)
{
	int len;

	len = 0;
	while (input[0] == SINGLE_QUOTE && input[len] && input[len] != SINGLE_QUOTE)
		len++;
	while (input[0] == DOUBLE_QUOTE && input[len] && input[len] != DOUBLE_QUOTE)
	{
		if (input[len] == BKSLASH && input[len + 1] 
			&& (input[len + 1] == DOUBLE_QUOTE || input[len + 1] == BKSLASH))
			len += 2;
		else
			len++;
	}
	if (input[len] && input[len] == input[0])
		len++;
	return (len);
}

	int
token_len(char *input)
{
	int i;

	i = 0;
	while (input[i] && !(IFS(input[i]) || RD(input[i]) || SEPARATOR(input[i])))
	{
		if (QUOTE(input[i]) && !(input[i - 1] && input[i - 1] == BKSLASH))
			i += quote_len(&input[i]);
		else if (input[i] == BKSLASH && input[i + 1])
			i += 2;
		else
			i++;
	}
	return (i);
}

	int
get_rd_type(t_rd *rd, char *input)
{
	int	i;
	int rd_type;

	i = 0;
	if (input[0] == '<')
		rd_type = RD_IN;
	else if (input[1] && input[0] == input[1])
		rd_type = APP_RD_OUT;
	else
		rd_type = RD_OUT;
	if (rd_type == RD_IN || rd_type == RD_OUT)
		i++;
	else
		i += 2;
	while (input[i] && IFS(input[i]))
		i++;
	if (rd)
		rd->type = rd_type;
	return (i);
}

	int
get_rd_value(t_rd *rd, char *input)
{
	int	len;
	int	i;

	i = 0;
	len = token_len(input);
	if (!(rd->value = malloc(sizeof(char *) * (len + 1))))
			return (0);
	while (i < len)
	{
		rd->value[i] = input[i];
		i++;
	}
	rd->value[len] = '\0';
	return (len);
}

	int
get_cmd_rd(t_cmd *cmd, t_rd *rd, char *input)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (rd)
		get_cmd_rd(cmd, rd->next, input);
	else
	{
		if (!input || (input && !(rd = malloc(sizeof(t_rd*)))))
        	return (0);
		i += get_rd_type(rd, input);
		i += get_rd_value(rd, &input[i]);
	}
	rd->next = NULL;
	return (i);
}

	int
cmd_len(t_cmd *cmd, char *input)
{
	int	i;

	i = 0;
	ft_printf("\nPARSE10");
		
	while (input[i] && !(input[i] == ';' || input[i] == '|'))
	{
		ft_printf("\nPARSE14");
			
		if (token_len(&input[i]))
		{
			cmd->argc++;
			ft_printf("\nPARSE15");
			i += token_len(&input[i]);
		}
		else if (RD(input[i]))
			i += get_cmd_rd(cmd, cmd->rd, &input[i]);
		else
			i++;
	}
	if (input[i] && (input[i] == ';' || input[i] == '|'))
		i++;
	return (i);
}

	void
get_cmd_argv(t_cmd *cmd, char *input, int cmd_len)
{
	int	i;
	int	j;
	int	len;

	i = 0;
	j = 0;
	len = 0;
	while (input[i] && i < cmd_len)
	{
		if ((len = token_len(&input[i])))
		{
			cmd->argv[j] = malloc(sizeof(char *) * (len + 1));
			cmd->argv[j] = ft_strncpy(cmd->argv[j], &input[i], len);
			cmd->argv[j][len] = '\0';
			i += len;
			j++;
		}
		else if (RD(input[i]))
		{
			i += get_rd_type(NULL, &input[i]);
			i += token_len(&input[i]);
		}
		else
			i++;
	}
}

	t_cmd*
parse_input(char *input)
{
	t_cmd	*cmd = NULL;
	int		len;
	
	ft_printf("\nPARSE01");
	cmd = init_cmd(input);
	if (!cmd)
		return (NULL);//remplacer par un cas d'erreur ? 
	ft_printf("\nPARSE03");
	ft_printf("\nARGC_PARSE = %d", cmd->argc);
    
	len = cmd_len(cmd, input);
	ft_printf("\nARGC = %d", cmd->argc);
	ft_printf("\nPARSE05");
	if (input[len - 1] == '|')
		cmd->pipe++;
	ft_printf("\nPARSE06");
	if (!input || (input && !(cmd->argv = malloc(sizeof(char **) * cmd->argc))))
        return (NULL);//idem 
	ft_printf("\nPARSE07");
	get_cmd_argv(cmd, input, len);
	ft_printf("\nPARSE08");
	if (input[len])//gérer l'erreur 2 ';' a suivre
		cmd->next = parse_input(&input[len]);
	ft_printf("\nPARSE09");
	return (cmd);
}
	/*
	while(input[i])
	{
		i += get_cmd(cmd->next, &input[i]); 
		i++;
	}
	cmd_count = ft_cmd_count(input);
	int			i;

	i = 0;
	while(input[i])
	{
		i++;
	}
	tokens = split_input(input);
	tokens = deal_quotes(tokens);
	check_tokens(tokens);
	i = 0;
	while (tokens[i] && !ft_strcmp(tokens[i], ";"))
		i++;
	list = NULL; 
	list = cmd_list(&tokens[i]);
	return (list);
}
*/