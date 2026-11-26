/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 19:06:04 by nicpinar          #+#    #+#             */
/*   Updated: 2024/10/01 19:06:04 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "include/minishell.h"

volatile sig_atomic_t	g_exit_code = 0;

static void	parse_and_execute(char **line, t_shell *shell)
{
	t_sections	*first_node;

	add_history(*line);
	first_node = parsing_main(line, shell->envp);
	shell->sections = first_node;
	if (shell->sections)
	{
		execute_pipeline(&shell);
		shell->sections = first_node;
		free_sections(&shell->sections);
	}
}

int	main(int argc, char **argv, char **envp)
{
	char		*line;
	t_shell		shell_stack;
	t_shell		*shell;

	(void)argc;
	(void)argv;
	shell_stack = init_env(envp);
	shell = &shell_stack;
	increment_shlvl(shell);
	while (1)
	{
		setup_signals();
		line = ft_readline(0);
		if (line == NULL)
		{
			free_strs(shell->envp);
			exit_shell(NULL);
		}
		if (*line != '\0')
			parse_and_execute(&line, shell);
		if (line)
			free(line);
	}
	return (0);
}
