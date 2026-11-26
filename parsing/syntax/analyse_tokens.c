/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   analyse_tokens.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/20 19:24:28 by nicpinar          #+#    #+#             */
/*   Updated: 2025/02/25 16:04:40 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	analyse_pipe(t_tokentab *table, t_token **token, int i)
{
	if (token[i]->type == PIPE)
	{
		if (i == 0)
		{
			ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
			g_exit_code = 2;
			return (1);
		}
		else if (i + 1 < table->size)
		{
			if (token[i + 1]->type == PIPE)
			{
				ft_putstr_fd("syntax error near unexpected token `|'\n", 2);
				g_exit_code = 2;
				return (1);
			}
		}
		else if (i + 1 == table->size)
		{
			ft_putstr_fd("ntm on gere pas ca\n", 2);
			return (1);
		}
	}
	return (0);
}

static int	analyse_redirections(t_tokentab *table, t_token **token, int i)
{
	if (token[i]->type == REDIN || token[i]->type == REDOUT
		|| token[i]->type == REDAPP)
	{
		if (i + 1 == table->size)
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			g_exit_code = 2;
			return (1);
		}
		else if (token[i + 1]->type != FILENAME)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			g_exit_code = 2;
			ft_putstr_fd(token[i + 1]->str, 2);
			return (1);
		}
	}
	return (0);
}

static int	analyse_heredoc(t_tokentab *table, t_token **token, int i,
		t_parserstate *state)
{
	if (token[i]->type == HEREDOC)
	{
		if (i + 1 == table->size)
		{
			ft_putstr_fd("syntax error near unexpected token `newline'\n", 2);
			g_exit_code = 2;
			return (1);
		}
		else if (token[i + 1]->type != DELIMITER && token[i + 1]->type
			!= DELIMITER_Q)
		{
			ft_putstr_fd("minishell: syntax error near unexpected token ", 2);
			ft_putstr_fd(token[i + 1]->str, 2);
			ft_putstr_fd("\n", 2);
			g_exit_code = 2;
			return (1);
		}
		else if (token[i + 1]->type == DELIMITER || token[i
				+ 1]->type == DELIMITER_Q)
		{
			if (gen_heredoc_prompt(token[i], token[i + 1], state) == 1)
				return (1);
		}
	}
	return (0);
}

static void	tilde_case(t_token **token, int i, t_parserstate *state)
{
	int	j;

	j = 0;
	if (token[i]->str[0] == '~')
	{
		if (token[i]->str[1] == '/')
		{
			join_path(token, i, state, 0);
			return ;
		}
	}
	while (token[i]->str[j])
	{
		if (token[i]->str[j] == '=' && token[i]->str[j + 1] == '~')
		{
			join_path(token, i, state, j + 1);
			return ;
		}
		j++;
	}
}

int	analyse_tokens(t_tokentab *table, t_parserstate *state)
{
	int		i;
	t_token	**token;

	i = 0;
	token = table->tokens;
	while (i < table->size)
	{
		if (analyse_redirections(table, token, i))
			return (1);
		if (analyse_pipe(table, token, i))
			return (1);
		if (analyse_heredoc(table, token, i, state))
			return (1);
		tilde_case(token, i, state);
		i++;
	}
	return (0);
}
