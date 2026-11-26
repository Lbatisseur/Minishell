# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: nicpinar <nicpinar@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2024/10/11 22:30:31 by nicpinar          #+#    #+#              #
#    Updated: 2025/02/24 21:21:47 by nicpinar         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #

NAME        = minishell
CC          = gcc
CFLAGS      = -Wall -Wextra -Werror -g
SRCDIR      = .
INCDIR      = include
INITDIR	 	= init
LIBFTDIR    = libft
PARSDIR     = parsing
SIGNDIR     = signal
UTILSDIR    = utils
EXECDIR 	= exec
BUILTINSDIR = $(EXECDIR)/builtins
CONVDIR		= $(PARSDIR)/convert
PARSERDIR   = $(PARSDIR)/parser
TOKENDIR    = $(PARSDIR)/token_manip
SYNTAX		= $(PARSDIR)/syntax
HEREDIR	 	= $(PARSDIR)/heredoc

OBJDIR      = obj

SRC = $(SRCDIR)/main.c \
	  $(INITDIR)/prompt.c \
	  $(INITDIR)/env.c \
      $(PARSDIR)/parsing_main.c \
      $(SYNTAX)/analyse_line.c \
      $(SYNTAX)/analyse_tokens.c \
	  $(HEREDIR)/heredoc.c \
	  $(CONVDIR)/convert.c \
      $(PARSERDIR)/define_type.c \
      $(PARSERDIR)/expansions.c \
      $(PARSERDIR)/tokenizer.c \
	  $(PARSERDIR)/define_binpath.c \
      $(TOKENDIR)/token_handling.c \
      $(TOKENDIR)/token_tab_handling.c \
      $(SIGNDIR)/setup.c \
	  $(SIGNDIR)/handler.c \
      $(UTILSDIR)/free_memory.c \
      $(UTILSDIR)/parsing_utils.c \
	  $(UTILSDIR)/parsing_utils2.c \
      $(UTILSDIR)/print.c \
	  $(UTILSDIR)/str_manip.c \
	  $(BUILTINSDIR)/cd.c \
	  $(BUILTINSDIR)/echo.c \
	  $(BUILTINSDIR)/env.c \
	  $(BUILTINSDIR)/exit.c \
	  $(BUILTINSDIR)/export.c \
	  $(BUILTINSDIR)/pwd.c \
	  $(BUILTINSDIR)/unset.c \
	  $(EXECDIR)/execution.c \
      $(EXECDIR)/redirection.c \
      $(EXECDIR)/export_utils.c \
      $(EXECDIR)/execution_bis.c \
      $(EXECDIR)/execution_utils.c \
	  $(EXECDIR)/exec_error.c \


SRCS = $(patsubst ./%,%,$(SRC))

OBJ = $(patsubst %.c,$(OBJDIR)/%.o,$(SRCS))

LIBS     = -lreadline -L /opt/homebrew/opt/readline/lib -L $(LIBFTDIR) -lft
INCLUDES = -I $(INCDIR) -I $(LIBFTDIR) -I /opt/homebrew/opt/readline/include


VPATH = $(sort $(dir $(SRCS)))

all: $(NAME)

$(NAME): $(OBJ)
	@$(MAKE) -C $(LIBFTDIR)
	$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(LIBS)

$(OBJDIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -rf $(OBJDIR)
	@$(MAKE) -C $(LIBFTDIR) clean

fclean: clean
	rm -f $(NAME)
	@$(MAKE) -C $(LIBFTDIR) fclean

re: fclean all

.PHONY: all clean fclean re
