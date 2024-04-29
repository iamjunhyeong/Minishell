/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pipe_control.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: heerpark <heerpark@student.42seoul.kr>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/19 13:13:59 by heerpark          #+#    #+#             */
/*   Updated: 2024/04/29 02:00:39 by heerpark         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	close_all_pipes(int **pipes, int n)
{
	int	i;

	i = 0;
	while (i < n)
	{
		close(pipes[i][0]);
		close(pipes[i][1]);
		i++;
	}
}

void	first_child(t_head *head, int **pipes, char **envp, int i)
{
	// close(pipes[i][0]);
	dup2(pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipes, head->size - 1);
	set_inout(head->processes[i], pipes, i, 1);
	if (is_builtin(head->processes[i]->exec_cmd))
	{
		run_builtin(head, head->processes[i]->exec_cmd);
		exit(0);
	}
	if (is_filepath(head->processes[i]->exec_cmd))
	{
		if (execve(head->processes[i]->exec_cmd[0], \
		head->processes[i]->exec_cmd, envp) == -1)
			perror_exit("file exe execve error");
		return ;
	}
	if (execve(head->processes[i]->exec_path, \
	head->processes[i]->exec_cmd, envp) == -1)
		perror_exit("execve error");
}

void	last_child(t_head *head, int **pipes, char **envp, int i)
{
	dup2(pipes[i - 1][0], STDIN_FILENO);
	close_all_pipes(pipes, head->size - 1);
	set_inout(head->processes[i], pipes, i, 0);
	if (is_builtin(head->processes[i]->exec_cmd))
	{
		run_builtin(head, head->processes[i]->exec_cmd);
		exit(0);
	}
	if (is_filepath(head->processes[i]->exec_cmd))
	{
		if (execve(head->processes[i]->exec_cmd[0], \
		head->processes[i]->exec_cmd, envp) == -1)
			perror_exit("file exe execve error");
		return ;
	}
	if (execve(head->processes[i]->exec_path, \
	head->processes[i]->exec_cmd, envp) == -1)
		perror_exit("execve error");
}

void	mid_child(t_head *head, int **pipes, char **envp, int i)
{
	dup2(pipes[i - 1][0], STDIN_FILENO);
	dup2(pipes[i][1], STDOUT_FILENO);
	close_all_pipes(pipes, head->size - 1);
	set_inout(head->processes[i], pipes, i, 1);
	if (is_builtin(head->processes[i]->exec_cmd))
	{
		run_builtin(head, head->processes[i]->exec_cmd);
		exit(0);
	}
	if (is_filepath(head->processes[i]->exec_cmd))
	{
		if (execve(head->processes[i]->exec_cmd[0], \
		head->processes[i]->exec_cmd, envp) == -1)
			perror_exit("file exe execve error");
		return ;
	}
	if (execve(head->processes[i]->exec_path, \
	head->processes[i]->exec_cmd, envp) == -1)
		perror_exit("execve error");
}

void	parent(int **pipes, int i)
{
	if (i > 0)
	{
		close(pipes[i - 1][0]);
		close(pipes[i - 1][1]);
	}
}

void	wait_process(int child_num)
{
	int	count;
	int	status;
	int	pid;

	count = 0;
	while (count < child_num)
	{
		pid = wait(&status);
		printf("process end: %d\n", pid);
		if (pid == -1)
		{
			perror_exit("wait error");
		}
		if (WIFEXITED(status))
		{
			sig = WEXITSTATUS(status);
			printf("error code %d\n", sig);
		}
		if (WIFSIGNALED(status))
		{
			sig = WTERMSIG(status);
			printf("sig error code %d\n", sig);
		}
		count++;
	}
	printf("wait end\n");
}
