/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/21 17:23:18 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/03/04 18:44:38 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	handle_child(t_shell *shell, t_sections *sections, int prev_fd,
	int is_last_cmd)
{
	setup_child_signals();
	setup_pipes(shell, prev_fd, is_last_cmd);
	if (sections->filename)
		handle_redirection(sections);
	if (is_builtin(sections->args[0]))
		exit(exec_builtin(shell));
	else
		execute_command(sections, shell->envp);
}

void	fork_and_execute(t_shell *shell, t_sections *sections, int prev_fd,
		int is_last_cmd)
{
	if (pipe(sections->fd_pipe) == -1)
	{
		perror("pipe");
		exit(EXIT_FAILURE);
	}
	unset_signals();
	shell->pid = fork();
	if (shell->pid == -1)
	{
		perror("Error with fork");
		g_exit_code = errno;
		exit(EXIT_FAILURE);
	}
	if (shell->pid == 0)
		handle_child(shell, sections, prev_fd, is_last_cmd);
	setup_parent_signals();
	close(sections->fd_pipe[1]);
	if (prev_fd != -1)
		close(prev_fd);
}

static void	execute_section(t_shell *bash, int *prev_fd)
{
	if (bash->sections->args && is_builtin(bash->sections->args[0])
		&& bash->sections->next == NULL)
		execute_builtin(bash, prev_fd);
	else if (bash->sections->args || bash->sections->redir)
		fork_and_execute(bash, bash->sections, *prev_fd,
			bash->sections->next == NULL);
}

static void	close_fds(t_shell *bash, int *prev_fd)
{
	if (*prev_fd != -1)
		close(*prev_fd);
	if (bash->sections->next)
	{
		close(bash->sections->fd_pipe[1]);
		*prev_fd = bash->sections->fd_pipe[0];
	}
}

void	execute_pipeline(t_shell **shell)
{
	t_shell		*bash;
	int			prev_fd;
	int			status;

	bash = *shell;
	prev_fd = -1;
	while (bash->sections)
	{
		bash->sections->args = find_exitcode(bash->sections->args);
		if (bash->sections->next && pipe(bash->sections->fd_pipe) == -1)
		{
			perror("Error with pipe");
			g_exit_code = errno;
			exit(EXIT_FAILURE);
		}
		execute_section(bash, &prev_fd);
		close_fds(bash, &prev_fd);
		bash->sections = bash->sections->next;
	}
	while (waitpid(-1, &status, 0) > 0)
		update_exit_status(status);
	unlink("heredoc.txt");
}
