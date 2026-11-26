/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   prompt.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 20:44:14 by nicpinar          #+#    #+#             */
/*   Updated: 2025/03/04 18:58:21 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	do_nothing(void)
{
	return (0);
}

void	exit_shell(char *line)
{
	if (line)
		free(line);
	rl_clear_history();
	exit(0);
}

char	*ft_readline(int type)
{
	char	*line;

	if (type == 0)
		line = readline("minishell> ");
	else
		line = readline("> ");
	if (g_exit_code == 130)
	{
		if (type == 1)
			return (NULL);
	}
	return (line);
}
