/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exit.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/29 03:20:22 by nbelouar42        #+#    #+#             */
/*   Updated: 2025/02/24 21:12:43 by nicpinar         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../include/minishell.h"

static int	check_out_of_range(int neg, unsigned long long num, int *error)
{
	if ((neg == 1 && num > LONG_MAX) || (neg == -1
			&& num > -(unsigned long)LONG_MIN))
		*error = 1;
	return (*error);
}

static unsigned long long	ft_ull_atoi(const char *str, int *error)
{
	unsigned long long	num;
	int					neg;
	int					i;

	num = 0;
	neg = 1;
	i = 0;
	while (str[i] && is_whitespace(str[i]))
		i++;
	if (str[i] == '+')
		i++;
	else if (str[i] == '-')
	{
		neg *= -1;
		i++;
	}
	while (str[i] && ft_isdigit(str[i]))
	{
		num = (num * 10) + (str[i] - '0');
		if (check_out_of_range(neg, num, error))
			break ;
		i++;
	}
	return (num * neg);
}

static int	get_exit_code(char *error_code, int *error)
{
	int					i;
	unsigned long long	code;

	i = 0;
	if (!error_code)
		return (0);
	while (error_code[i] && is_whitespace(error_code[i]))
		i++;
	if (error_code[i] == '+' && error_code[i] == '-')
		i++;
	while (error_code[i] && error_code[i] >= '0' && error_code[i] <= '9')
		i++;
	while (error_code[i] && is_whitespace(error_code[i]))
		i++;
	if (error_code[i])
		*error = 1;
	code = ft_ull_atoi(error_code, error);
	return (code % 256);
}

int	ft_exit(t_sections *shell)
{
	int	error;
	int	size;

	error = 0;
	size = ft_strs_len(shell->args);
	if (size == 1)
	{
		free_sections(&shell);
		g_exit_code = 0;
		write(1, "exit\n", 5);
		exit(g_exit_code);
	}
	if (size > 2)
		return (ft_error("bash: exit: too many arguments\n"));
	else
	{
		g_exit_code = get_exit_code(shell->args[1], &error);
		if (error == 1)
			return (ft_error("bash: exit: "), ft_error(shell->args[1]),
				ft_error(": numeric argument required\n"));
		else
			write(1, "exit\n", 5);
	}
	free_sections(&shell);
	exit(g_exit_code);
}

char	**find_exitcode(char **args)
{
	int	i;

	i = 0;
	if (!args)
		return (NULL);
	while (args[i])
	{
		if (!ft_strncmp(args[i], "$?", 2))
		{
			free(args[i]);
			args[i] = ft_itoa(g_exit_code);
		}
		i++;
	}
	return (args);
}
