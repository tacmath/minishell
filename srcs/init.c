/* ************************************************************************** */
/*                                                          LE - /            */
/*                                                              /             */
/*   init.c                                           .::    .:/ .      .::   */
/*                                                 +:+:+   +:    +:  +:+:+    */
/*   By: mtaquet <marvin@le-101.fr>                 +:+   +:    +:    +:+     */
/*                                                 #+#   #+    #+    #+#      */
/*   Created: 2019/06/06 14:10:39 by mtaquet      #+#   ##    ##    #+#       */
/*   Updated: 2019/06/06 15:23:39 by mtaquet     ###    #+. /#+    ###.fr     */
/*                                                         /                  */
/*                                                        /                   */
/* ************************************************************************** */

#include "minishell.h"

int change_env(char **env, char *var, char *new)
{
	int n;

	n = -1;
	while (env[++n])
		if (!ft_strncmp(env[n], var, ft_strlen(var)) && env[n][ft_strlen(var)] == '=')
		{
			free(env[n]);
			env[n] = ft_super_join(3, var, "=", new);
			break ;
		}
	return (1);
}

int shell_init(t_shell *shell)
{
	int n;
	char *tmp;

	shell->home = 0;
	n = -1;
	while (environ[++n])
		if (!ft_strncmp(environ[n], "HOME=", 5))
		{
			shell->home = ft_strdup(&environ[n][5]);
			break ;
		}
	if (!shell->home)
		shell->home = getcwd(0, 0);
	shell->last_dir = 0;
	n = -1;
	while (environ[++n])
		;
	if (!(shell->shell_env = ft_memalloc(sizeof(char*) * ++n)))
		return (0);
	n = -1;
	while (environ[++n])
		shell->shell_env[n] = ft_strdup(environ[n]);
	n = -1;
	while (environ[++n])
		if (!ft_strncmp(environ[n], "_=", 2))
		{
			tmp = ft_strdup(&environ[n][2]);
			change_env(shell->shell_env, "SHELL", tmp);
			free(tmp);
			break ;
		}
	return (1);
}
