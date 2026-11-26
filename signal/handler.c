/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   handler.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/15 17:50:11 by nicpinar          #+#    #+#             */
/*   Updated: 2025/03/04 18:56:08 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../include/minishell.h"

void	sigint_parent(int sig)
{
	g_exit_code = sig + 128;
	ft_putstr_fd("\n", STDOUT_FILENO);
	rl_on_new_line();
	rl_replace_line("", 0);
}

void	sigint_main(int sig)
{
	g_exit_code = sig + 128;
	write(STDERR_FILENO, "\n", 1);
	rl_on_new_line();
	rl_replace_line("", 0);
	rl_redisplay();
}

void	sigint_heredoc(int sig)
{
	g_exit_code = sig + 128;
	rl_replace_line("", 0);
	write(STDERR_FILENO, "\n", 1);
	rl_done = 1;
	rl_on_new_line();
}
