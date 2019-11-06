/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   exe.c                                            .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:20 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/11/06 16:04:26 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int		run_command(char *path, char **av, t_shell *shell)
{
	pid_t	father;
	char	*tmp;
	char	*path_tmp;

	tcsetattr(0, TCSANOW, &shell->old_term);
	if (path[0] == '.')
	{
		path_tmp = getcwd(0, 0);
		tmp = ft_super_join(3, path_tmp, "/", path);
		free(path_tmp);
	}
	else
		tmp = ft_strdup(path);
	change_env(shell->shell_env, "_", tmp);
	free(tmp);
	father = fork();
	if (father == 0)
		if (execve(path, av, shell->shell_env) == -1)
			ft_putendl("command failed");
	wait(NULL);
	tcsetattr(0, TCSANOW, &shell->shell_term);
	return (1);
}

int		run_with_path(char **av, t_shell *shell)
{
	char	**path;
	char	*tmp;
	int		n;

	path = get_all_path(shell->shell_env);
	n = -1;
	while (path[++n])
	{
		tmp = ft_strjoin(path[n], av[0]);
		if (!access(tmp, F_OK))
		{
			if (!access(tmp, X_OK))
				run_command(tmp, av, shell);
			else
				print_file_error("minishell: permission denied: ", av[0]);
			free(tmp);
			break ;
		}
		free(tmp);
	}
	if (!path[n])
		print_file_error("minishell: command not found: ", av[0]);
	free_av(path);
	return (1);
}

int		run_non_builtin(char **av, t_shell *shell)
{
	if (!access(av[0], F_OK))
	{
		if (!access(av[0], X_OK))
			run_command(av[0], av, shell);
		else
			print_file_error("minishell: permission denied: ", av[0]);
	}
	else
		run_with_path(av, shell);
	return (1);
}
