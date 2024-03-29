/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heerpark <heerpark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:27:44 by junhyeop          #+#    #+#             */
/*   Updated: 2024/03/26 21:15:13 by heerpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <unistd.h>
# include <stdlib.h>
# include <fcntl.h>
# include <sys/types.h>
# include <sys/errno.h>
# include <sys/wait.h>
# include "../libft/libft.h"
# include "../libft/get_next_line/get_next_line.h"

# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <readline/readline.h>
# include <readline/history.h>
# include <unistd.h>
# include <termios.h>

typedef struct s_flag
{
	int	quote;
	int	dquote;
	int pipe;

}	t_flag;

typedef struct s_token
{
	char			*cmd;
	int				redir_flag;
	struct s_token	*next;
	struct s_token	*prev;
}	t_token;

typedef struct s_list
{
	t_token			*token;
	struct s_list	*next;
	struct s_list	*prev;
}	t_list;

typedef struct s_process
{
	int		re_infile_fd;
	int		re_outfile_fd;
	int		re_append_fd;
	int		heredoc_fd;
	int		is_built;
	char	*heredoc_filename;
	char	*cmd;
	char	*exec_path;
	char	**exec_cmd;
	char	*builtin_cmds;
}	t_process;

typedef struct s_head {
	int				size;
	char			**exec_rm_cmd;
	char			*exec_rm_path;
	struct s_list	*top;
	t_process		**processes;
}	t_head;

//parsing func
void		error_msg(int type);

void		add_cmd(t_head *head, char *line, int pipe_flag);
void		add_token(t_token **lst, char *cmd);

t_token		*make_token(char *command);
t_head		*init_head(void);
t_token		*token_new(char *command, int flag);

t_list		*cmd_list_new(char *command);
void		ft_lst_add(t_head *head, t_list *new);
void		ft_token_add(t_token **lst, t_token *new);

char		**split_pipe(char const *s);
t_token		*split_space(char *s, char space);	// pipe 단위로 나눈 것 -> 공백 단위로 나눔

void		free_list(t_head *head);


void		parse(char *str, t_head *head);

//exe func

	//exe_parsing.c, exe_parsing2.c)
char		**get_exec_paths(char **envp, char **cmds, int path_size);
char		*get_envpath(char **envp);
char		**get_cmd_head(char **argv, int start_idx, int end_idx);
char		***make_cmds(char **argv, int start_idx, int end_idx);
void		free_cmds(int cmds_size, char ***cmds);

void		free_splited(char **splited);
int			**make_pipe(int child_num);
void		free_pipe(int **pipe, int child_num);
void		perror_exit(char *msg);

	//pipe_control
void		wait_process(int child_num);
void		first_child(t_process *process, int **pipes, char **envp, int i);
void		last_child(t_process *process, int **pipes, char **envp, int i);
void		mid_child(t_process *process, int **pipes, char **envp, int i);
void		parent(int **pipes, int i);

	//get_fd
int			get_infile(char	*file_name);
int			get_heredoc(t_process *process, char *limiter);
int			get_outfile(char *file_name);
int			get_append(char *file_name);
void		init_fd(t_process *process);

	//here_doc
void		kill_heredoc(t_head *head, char **envp);
char		*make_infile(char *limiter);

	//list_to_processes_utils
void		fill_elem(t_token *temp, t_process *process, char *cmd, int flag);
void		set_fd(t_process *process, char *file_name, int redir_flag);
int			get_redir_flag(char	*token);
void		set_process(t_process *process, char **path);
t_process	*get_process(t_list *line, char **path);

	//processes_exe
void		get_processes(t_head *head, char **envp);
void		set_inout(t_process *process, int **pipes, int i, int close_sig);
void		start_process(t_process **pss, char **envp);
void		start_processes(t_process **pss, char **envp, int **pipes, int n);
void		exe(t_head *head, char **envp);

	//env.c
int			is_builtin(char *cmd);
void		run_builtin(char *cmd, char **envp);

#endif