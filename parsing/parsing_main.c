/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing_main.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 20:12:45 by nicpinar          #+#    #+#             */
/*   Updated: 2025/03/07 21:27:40 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

static int	init_parserstate(t_parserstate *state, char *line, char **dict)
{
	state->current_token = NULL;
	state->line = line;
	state->index = 0;
	state->table = create_token_table();
	if (!state->table)
		malloc_error("init table failed\n", state, NULL);
	state->dict = dict;
	return (0);
}

t_sections	*parsing_main(char **line, char **dict)
{
	t_tokentab		*table;
	t_parserstate	state;
	t_sections		*sections;

	sections = NULL;
	if (analyse_line(*line) || init_parserstate(&state, *line, dict))
		return (NULL);
	table = tokenizer(&state);
	print_token_table(table);
	if (*line)
	{
		free(*line);
		*line = NULL;
	}
	if (analyse_tokens(table, &state))
	{
		destroy_token_table(table);
		return (NULL);
	}
	if (convert_to_list(&sections, table))
		malloc_error("convert_to_list failed\n", &state, NULL);
	print_sections(sections);
	destroy_token_table(table);
	if (sections)
		define_binpath(&sections, dict);
	return (sections);
}
