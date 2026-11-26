/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nassimbelouar <nassimbelouar@student.42    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/10/01 18:17:14 by nicpinar          #+#    #+#             */
/*   Updated: 2025/12/13 18:46:46 by nassimbelou      ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

extern volatile sig_atomic_t	g_exit_code;

# define LONG_MAX 9223372036854775807
# define LONG_MIN 9223372036854775807

typedef struct s_sections		t_sections;

typedef struct s_shell
{
	t_sections					*sections;
	char						**envp;
	pid_t						pid;
}								t_shell;

typedef struct s_sections
{
	char						**args;
	char						*path_bin;
	char						**redir;
	char						**filename;
	char						**heredoc;
	char						*path;
	int							fd_in;
	int							fd_out;
	int							fd_pipe[2];
	char						*pipe;
	struct s_sections			*next;
}								t_sections;

typedef enum e_type
{
	UNKNOWN,
	PIPE,
	REDIN,
	REDOUT,
	REDAPP,
	FILENAME,
	HEREDOC,
	DELIMITER,
	DELIMITER_Q,
	COMMAND,
	ARG
}								t_type;

typedef struct token
{
	char						*str;
	char						**expanded_split;
	int							size;
	int							mem_size;
	t_type						type;
}								t_token;

typedef struct tokentab
{
	t_token						**tokens;
	int							size;
	int							mem_size;
}								t_tokentab;

typedef struct s_parserstate
{
	char						**dict;
	t_tokentab					*table;
	t_token						*current_token;
	char						*line;
	int							index;
}								t_parserstate;

//-----------------------------INIT-----------------------------------------//

//prompt.c
char							*ft_readline(int type);
void							exit_shell(char *line);
int								do_nothing(void);

//env.c
void							increment_shlvl(t_shell *shell);
t_shell							init_env(char **envp);

//--------------------------------PARSING-------------------------------------//

//parsing_main.c
t_sections						*parsing_main(char **line, char **dict);

//------CONVERT-----//

//convert.c
int								convert_to_list(t_sections **sections,
									t_tokentab *table);

//------HEREDOC-----//
//heredoc.c
int								gen_heredoc_prompt(t_token *token, t_token *del,
									t_parserstate *state);

//------SYNTAX-----//

//analyse_line.c
int								analyse_line(char *line);
//analyse_tokens.c
int								analyse_tokens(t_tokentab *table,
									t_parserstate *state);

//------PARSER-----//

//define_type.c
void							define_op_type(t_parserstate *state, char c,
									char next);
void							define_type(t_parserstate *state, int quote);

//expansions.c
int								handle_expansion(t_parserstate *state,
									int quote);
char							*ft_expand(char *str, t_parserstate *state);
char							*parse_dict(char *var_name, char **dict);

//tokeniser.c
t_tokentab						*tokenizer(t_parserstate *state);

//binpath.c
void							define_binpath(t_sections **sections,
									char **dict);

//-----TOKEN_MANIP-----//

//token_handling.c
t_token							*create_token(t_parserstate *state);
void							destroy_token(t_token *t);
void							push_char(t_token *t, char c,
									t_parserstate *state);

//token_tab_handling.c
t_tokentab						*create_token_table(void);
void							destroy_token_table(t_tokentab *t);
void							push_token(t_tokentab *t, t_token *tok,
									t_parserstate *state);
void							print_tokens(t_tokentab *t);

//-----------------------------SIGNAL-----------------------------------------//

//signal.c
void							setup_parent_signals(void);
void							setup_child_signals(void);
void							unset_signals(void);
void							setup_signals(void);
void							setup_heredoc_signals(void);

//handler.c
void							sigint_parent(int sig);
void							sigint_main(int sig);
void							sigint_heredoc(int sig);

//-----------------------------UTILS-----------------------------------------//

//free_memory.c
void							malloc_error2(char *str, char **split,
									t_sections **sections, char **dict);
void							free_sections(t_sections **sections);
int								free_strs(char **strs);
void							malloc_error(char *str, t_parserstate *state,
									void **local);
void							free_and_reassign(t_sections *curr,
									char *new_arg);

//parsing_utils.c
int								prev_heredoc(t_parserstate *state);
int								is_operator(char c);
int								is_valid_exp(char c, unsigned int next);
int								ft_strcmp(const char *s1, const char *s2);
int								to_find_str(char *str, char *to_find);

//parsing_utils2.c
int								is_only_slashes(char *str);
int								is_only_whitespaces(char *str);
int								is_whitespace(char c);
int								ft_strs_len(char **strs);

//print.c
void							print_token_table(t_tokentab *table);
void							print_sections(t_sections *sections);

//str_manip.c
char							**get_dict(char **envp, int copy);
char							**add_to_array(char **array, char *str);
void							*ft_realloc(void *ptr, size_t old_size,
									size_t new_size, t_parserstate *state);
int								ft_strs_len(char **strs);
void							join_path(t_token **token, int i,
									t_parserstate *state, int j);

//-----------------------------EXEC-----------------------------------------//

//execution_bis.c
int								exec_builtin(t_shell *shell);
void							update_exit_status(int status);
void							execute_builtin(t_shell *shell, int *prev_fd);
int								ft_error(char *error);

//execution_utils.c
void							restore_stdin_stdout(int stdin, int stdout);
void							setup_pipes(t_shell *shell, int prev_fd,
									int last_cmd);
void							execute_command(t_sections *shell, char **env);

//execution.c
void							fork_and_execute(t_shell *shell,
									t_sections *sections, int prev_fd,
									int is_last_cmd);
void							execute_pipeline(t_shell **shell);

//redirection.c
void							handle_redirection(t_sections *node);
int								redir_out(char *filename);
int								redir_in(char *filename);
int								redir_app(char *filename);

//export_utils.c
void							free_tab(char **tab);
char							*ft_strjoin_bis(char *s1, char *s2);
int								is_valid_key(char *str);
int								is_builtin(char *str);

//exec_error.c
void							check_error_ex(t_sections *shell, char **env);

//-------builtins-------//

//pwd.c
int								ft_pwd(void);

//env.c
int								ft_env(char **env, char **token,
									int export_mode);
char							*get_env_value(t_shell *shell, char *str,
									int malloc);

//cd.c
int								ft_cd(t_shell *shell);
int								ft_is_dir(char *path);

//echo.c
int								ft_echo(t_sections *shell);

//export.c
int								ft_export(t_shell *shell, char **token);
char							**set_env_var(char **env, char *key,
									char *value);			
char							**realloc_env(char **env, int len);
char							**get_key_and_value(char *str);
int								get_env_index(char **env, char *key);

//unset.c
int								ft_unset(t_shell *shell);

//exit.c
int								ft_exit(t_sections *shell);
char							**find_exitcode(char **args);

#endif
