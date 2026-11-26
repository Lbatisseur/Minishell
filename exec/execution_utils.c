/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 08:45:51 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/20 16:59:50 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	restore_stdin_stdout(int stdin, int stdout)
{
	dup2(stdout, STDOUT_FILENO);
	dup2(stdin, STDIN_FILENO);
	close(stdin);
	close(stdout);
}

void	setup_pipes(t_shell *shell, int prev_fd, int is_last)
{
	if (prev_fd != -1)
	{
		dup2(prev_fd, STDIN_FILENO);
		close(prev_fd);
	}
	if (!is_last)
	{
		dup2(shell->sections->fd_pipe[1], STDOUT_FILENO);
	}
	close(shell->sections->fd_pipe[1]);
	close(shell->sections->fd_pipe[0]);
}

int	ft_strslen(char **strs)
{
	int	i;

	i = 0;
	while (strs[i])
		i++;
	return (i);
}

char	*heredoc_to_file(char **heredoc)
{
	int		fd;
	int		len;
	char	*filename;

	len = ft_strslen(heredoc);
	filename = ft_strjoin("heredoc", ".txt");
	fd = open(filename, O_CREAT | O_RDWR | O_TRUNC, 0644);
	if (fd == -1)
	{
		perror("open");
		g_exit_code = errno;
		exit(EXIT_FAILURE);
	}
	if (write(fd, heredoc[len - 1], ft_strlen(heredoc[len - 1])) == -1)
	{
		g_exit_code = errno;
		exit(EXIT_FAILURE);
	}
	if (write(fd, "\n", 1) == -1)
	{
		g_exit_code = errno;
		exit(EXIT_FAILURE);
	}
	close(fd);
	return (filename);
}

void	execute_command(t_sections *shell, char **env)
{
	char	*herename;

	check_error_ex(shell, env);
	if (!shell->args[1] && shell->heredoc)
	{
		herename = heredoc_to_file(shell->heredoc);
		shell->args = add_to_array(shell->args, herename);
	}
	if (execve(shell->path_bin, shell->args, env) == -1)
	{
		perror(shell->args[0]);
		exit(g_exit_code);
	}
	exit(g_exit_code);
}
