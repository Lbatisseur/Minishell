/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execution_bis.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/20 08:43:12 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 22:40:44 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

int	ft_error(char *error)
{
	ft_putendl_fd(error, STDERR_FILENO);
	errno = 0;
	return (0);
}

int	is_builtin(char *str)
{
	if (!ft_strncmp(str, "echo", 5) || !ft_strncmp(str, "cd", 3)
		|| !ft_strncmp(str, "pwd", 4) || !ft_strncmp(str, "export", 7)
		|| !ft_strncmp(str, "unset", 6) || !ft_strncmp(str, "env", 4)
		|| !ft_strncmp(str, "exit", 5))
		return (1);
	return (0);
}

int	exec_builtin(t_shell *shell_env)
{
	t_sections	*shell;

	shell = shell_env->sections;
	if (!ft_strncmp(shell->args[0], "cd", ft_strlen("cd")))
		return (ft_cd(shell_env));
	if (!ft_strncmp(shell->args[0], "echo", ft_strlen("echo")))
	{
		ft_echo(shell);
		return (0);
	}
	if (!ft_strncmp(shell->args[0], "exit", ft_strlen("exit")))
		return (ft_exit(shell));
	if (!ft_strncmp(shell->args[0], "export", ft_strlen("export")))
		return (ft_export(shell_env, shell->args));
	if (!ft_strncmp(shell->args[0], "env", ft_strlen("env")))
		return (ft_env(shell_env->envp, shell->args, 0));
	if (!ft_strncmp(shell->args[0], "unset", ft_strlen("unset")))
		return (ft_unset(shell_env));
	if (!ft_strncmp(shell->args[0], "pwd", ft_strlen("pwd")))
		return (ft_pwd());
	return (0);
}

void	update_exit_status(int status)
{
	unlink("heredoc.txt");
	if (WIFEXITED(status))
		g_exit_code = WEXITSTATUS(status);
	else if (WIFSIGNALED(status))
		g_exit_code = WTERMSIG(status) + 128;
}

void	execute_builtin(t_shell *shell, int *prev_fd)
{
	int	saved_stdout;
	int	saved_stdin;

	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	if (shell->sections->filename)
		handle_redirection(shell->sections);
	if (shell->sections->next != NULL)
	{
		if (pipe(shell->sections->fd_pipe) == -1)
		{
			g_exit_code = errno;
			restore_stdin_stdout(saved_stdin, saved_stdout);
			exit(EXIT_FAILURE);
		}
		if (dup2(shell->sections->fd_pipe[1], STDOUT_FILENO) == -1)
		{
			g_exit_code = errno;
			exit(EXIT_FAILURE);
		}
		close(shell->sections->fd_pipe[1]);
		*prev_fd = shell->sections->fd_pipe[0];
	}
	g_exit_code = exec_builtin(shell);
	restore_stdin_stdout(saved_stdin, saved_stdout);
}
