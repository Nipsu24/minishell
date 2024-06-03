/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mmeier <mmeier@student.hive.fi>            +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/03 12:39:08 by mmeier            #+#    #+#             */
/*   Updated: 2024/06/03 14:47:16 by mmeier           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "./libft/libft.h"
# include <signal.h>
# include <readline/readline.h>
# include <readline/history.h>

/*Linked list containing command structure extracted from tokens*/
typedef struct s_command
{
	char				*cmd;
	char				**args;
	char				*file_in;
	char				*file_out;
	struct s_command	*next;
}				t_command;

typedef enum s_token_type
{
	COMMAND,
	STRING,
	REDIRECT_IN,
	REDIRECT_OUT,
	REDIRECT_IN_DEL,
	REDIRECT_OUT_APP,
	FILE_NAME,
	PIPE,
	ENVAR,
	BUILTIN
}			t_token_type;

/*Holds information of a single token*/
typedef struct s_token
{
	char			*cnt;
	t_token_type	type;
}				t_token;

/*Struct containing all tokens generated from user input*/
typedef struct s_tokens
{
	t_token		*entry;
	int			num;	
}				t_tokens;

/*Overall struct for all relevant data of the shell*/
typedef struct s_data
{
	char		**temp_env;
	char		**tokens;
	char		*input;
	t_tokens	*token_list;
	t_command	*cmds;
}				t_data;

void	print_env(char **env);
char	**ft_tokenize(char const *s, char c, char str_flag);
void	ft_token_type(t_data *tokens, int i);
void	free_all(t_data *data);
int		ft_malloc_token(t_data *data);
char	**free_arr_rev(char **av, int j);
int		not_valid_input(char *str);
void	setup_signal(void);
int		between_quotes(char *input, int pos);
int		parse_cmds(t_data *data);

#endif