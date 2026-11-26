/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:16:25 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 21:12:32 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

int	is_n_option(char *str)
{
	int	i;

	i = 1;
	if (str[0] != '-')
		return (0);
	while (str[i] && str[i] == 'n')
		i++;
	if (!str[i])
		return (1);
	return (0);
}

int	ft_echo(t_sections *shell)
{
	int	i;
	int	is_n;

	is_n = 0;
	i = 0;
	while (shell->args[++i])
	{
		if (is_n_option(shell->args[i]) == 1)
			is_n = 1;
		if (is_n_option(shell->args[i]) == 0)
			break ;
	}
	while (shell->args[i])
	{
		ft_putstr_fd(shell->args[i], STDOUT_FILENO);
		if (shell->args[i + 1])
			ft_putchar_fd(' ', STDOUT_FILENO);
		i++;
	}
	if (!is_n)
		ft_putchar_fd('\n', STDOUT_FILENO);
	return (0);
}
