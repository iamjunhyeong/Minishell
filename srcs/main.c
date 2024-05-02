/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: junhyeop <junhyeop@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/16 17:10:35 by junhyeop          #+#    #+#             */
/*   Updated: 2024/05/02 17:58:47 by junhyeop         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
// gcc testsignal.c -lreadline -L/Users/sham/.brew/opt/readline/lib -I/Users/sham/.brew/opt/readline/include


int	check_white_space(char *str)
{
	int	i;

	i = 0;
	while (str[i])
	{
		if (str[i] != ' ')
			return (0);
		i++;
	}
	return (1);
}

int main(int argc, char **argv, char **envp)
{
	(void)argc;
	(void)argv;

    char			*str;
	t_head			*head;

	g_exit_status = -1;
    set_signal();
	head = init_head(envp);
	ft_printf("Welcome to minishell!\n");
    while (1)
    {

        str = readline("minishell$ ");
		// printf("input!!\n");
        if (!str)
			exit_signal();
        else if (*str == '\0' || check_white_space(str))
            free(str);
        else
        {
            add_history(str);
			parse(str, head);
			exe(head, envp);
			free_list(head);
            free(str);
        }
    }
    /* 함수종료 */
    return (0);
}

//file descriptor 싹다 닫아주는 함수 만드는 것도 좋을듯.